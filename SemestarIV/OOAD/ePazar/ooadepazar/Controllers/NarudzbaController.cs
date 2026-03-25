using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Rendering;
using Microsoft.EntityFrameworkCore;
using ooadepazar.Data;
using ooadepazar.Models;

namespace ooadepazar.Controllers
{
    public class NarudzbaController : Controller
    {
        private readonly ApplicationDbContext _context;
        private readonly UserManager<ApplicationUser> _userManager;
        private readonly IMailService _mailService;

        public NarudzbaController(ApplicationDbContext context, UserManager<ApplicationUser> userManager, IMailService mailService)
        {
            _context = context;
            _userManager = userManager;
            _mailService = mailService;
        }

        // GET: Narudzba
        [Authorize(Roles = "KurirskaSluzba, Admin")]
        public async Task<IActionResult> Index(string kurirskaSluzbaId = null)
        {
            var currentUser = await _userManager.GetUserAsync(User);
            var isAdmin = await _userManager.IsInRoleAsync(currentUser, "Admin");

            var narudzbeQuery = _context.Narudzba
                .Include(n => n.Artikal)
                .Include(n => n.Korisnik)
                .Include(n => n.KurirskaSluzba)
                .AsQueryable();

            // Ako nije admin, prikaži samo narudžbe te kurirske službe
            if (!isAdmin)
            {
                narudzbeQuery = narudzbeQuery.Where(n => n.KurirskaSluzba != null && n.KurirskaSluzba.Id == currentUser.Id);
            }
            // Ako je admin i odabrao je kurirsku službu za filtriranje
            else if (!string.IsNullOrEmpty(kurirskaSluzbaId))
            {
                narudzbeQuery = narudzbeQuery.Where(n => n.KurirskaSluzba != null && n.KurirskaSluzba.Id == kurirskaSluzbaId);
            }

            var narudzbe = await narudzbeQuery.ToListAsync();

            // Kreiranje liste kurirskih službi samo za admina
            List<SelectListItem> kuriri = new List<SelectListItem>();
            if (isAdmin)
            {
                var kurirskaSluzbaUsers = await _userManager
                    .GetUsersInRoleAsync("KurirskaSluzba");

                kuriri = kurirskaSluzbaUsers
                    .OrderBy(u => !string.IsNullOrEmpty(u.KurirskaSluzba) ? u.KurirskaSluzba : $"{u.Ime} {u.Prezime}")
                    .Select(u => new SelectListItem
                    {
                        Value = u.Id,
                        Text = !string.IsNullOrEmpty(u.KurirskaSluzba)
                               ? u.KurirskaSluzba
                               : $"{u.Ime} {u.Prezime}"
                    })
                    .ToList();
            }

            ViewBag.KurirskeSluzbe = kuriri;
            ViewBag.OdabranaKurirskaSluzba = kurirskaSluzbaId;
            ViewBag.IsAdmin = isAdmin;

            return View(narudzbe);
        }

        // GET: Narudzba/Details/5
        public async Task<IActionResult> Details(int? id)
        {
            if (id is null)
                return NotFound();

            var narudzba = await _context.Narudzba
                .Include(n => n.Artikal)
                    .ThenInclude(a => a.Korisnik)
                .Include(n => n.Korisnik)
                .Include(n => n.KurirskaSluzba)
                .FirstOrDefaultAsync(m => m.ID == id);

            if (narudzba is null)
                return NotFound();

            return View(narudzba);
        }

        [HttpGet]
        [Authorize]
        public async Task<IActionResult> Create(int id)
        {
            var artikal = await _context.Artikal.FirstOrDefaultAsync(a => a.ID == id);
            if (artikal == null) return NotFound();

            var korisnik = await _userManager.GetUserAsync(User);
            if (korisnik == null) return Unauthorized();

            artikal.Narucen = true;

            // Lista kurirskih službi na osnovu role - sortirana abecedno
            var kurirskaSluzbaUsers = await _userManager.GetUsersInRoleAsync("KurirskaSluzba");

            // Filtriraj kurirske službe - isključi trenutnog korisnika ako je i on kurirska služba
            var filtriranKuriri = kurirskaSluzbaUsers
                .Where(u => u.Id != korisnik.Id) // Isključi trenutnog korisnika
                .OrderBy(u => !string.IsNullOrEmpty(u.KurirskaSluzba) ? u.KurirskaSluzba : $"{u.Ime} {u.Prezime}")
                .Select(u => new SelectListItem
                {
                    Value = u.Id,
                    Text = !string.IsNullOrEmpty(u.KurirskaSluzba) ? u.KurirskaSluzba : $"{u.Ime} {u.Prezime}"
                }).ToList();

            // Provjeri da li ima dostupnih kurirskih službi
            if (!filtriranKuriri.Any())
            {
                TempData["ErrorMessage"] = "Trenutno nema dostupnih kurirskih službi za dostavu.";
                return RedirectToAction("Details", "Artikal", new { id = id });
            }

            ViewBag.ArtikalId = id;
            ViewBag.Korisnik = korisnik;
            ViewBag.KurirskeSluzbe = filtriranKuriri;

            return View();
        }

        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize]
        public async Task<IActionResult> Create(int id, string Lokacija, string KurirskaSluzbaId)
        {
            var korisnik = await _userManager.GetUserAsync(User);
            if (korisnik == null) return Unauthorized();

            var artikal = await _context.Artikal
                .Include(a => a.Korisnik)
                .FirstOrDefaultAsync(a => a.ID == id);
            if (artikal == null) return NotFound();

            var kurirskaSluzba = await _context.Users.FirstOrDefaultAsync(u => u.Id == KurirskaSluzbaId);
            if (kurirskaSluzba == null) return BadRequest("Morate izabrati kurirsku službu.");

            // Dodatna provjera da korisnik ne može odabrati sebe kao kurirsku službu
            if (KurirskaSluzbaId == korisnik.Id)
            {
                TempData["ErrorMessage"] = "Ne možete odabrati sebe kao kurirsku službu.";
                return RedirectToAction("Create", new { id = id });
            }

            // Provjeri da li je korisnik sa KurirskaSluzbaId stvarno kurirska služba
            var isKurirskaSluzba = await _userManager.IsInRoleAsync(kurirskaSluzba, "KurirskaSluzba");
            if (!isKurirskaSluzba)
            {
                TempData["ErrorMessage"] = "Odabrani korisnik nije kurirska služba.";
                return RedirectToAction("Create", new { id = id });
            }

            var narudzba = new Narudzba
            {
                DatumNarudzbe = DateTime.Now,
                Status = Status.Kreiran,
                Lokacija = Lokacija,
                Artikal = artikal,
                Korisnik = korisnik,
                KurirskaSluzba = kurirskaSluzba
            };

            artikal.Narucen = true;

            _context.Narudzba.Add(narudzba);

            // Kreiraj notifikaciju za vlasnika artikla
            if (artikal.Korisnik != null)
            {
                var notifikacija = new Notifikacija
                {
                    Sadrzaj = $"<a href='/Korisnik/{korisnik.Id}'>{korisnik.Ime} {korisnik.Prezime}</a> je naručio vaš artikal \"{artikal.Naziv}\".",
                    DatumObjave = DateTime.Now,
                    Procitana = false,
                    KorisnikId = artikal.Korisnik // vlasnik artikla
                };
                _context.Notifikacija.Add(notifikacija);
            }

            await _context.SaveChangesAsync();

            TempData["SuccessMessage"] = "Narudžba je uspješno kreirana!";
            return RedirectToAction("Index", "Home");
        }

        // GET: Narudzba/Edit/5
        public async Task<IActionResult> Edit(int? id)
        {
            if (id == null) return NotFound();

            var narudzba = await _context.Narudzba
                .Include(n => n.Artikal)
                .Include(n => n.Korisnik)
                .Include(n => n.KurirskaSluzba)
                .FirstOrDefaultAsync(n => n.ID == id);

            if (narudzba == null) return NotFound();

            ViewData["ArtikalID"] = new SelectList(_context.Artikal, "ID", "Naziv", narudzba.Artikal?.ID);
            ViewData["KorisnikID"] = new SelectList(_context.Users, "Id", "Ime", narudzba.Korisnik?.Id);
            ViewData["KurirskaSluzbaID"] = new SelectList(_context.Users, "Id", "Ime", narudzba.KurirskaSluzba?.Id);
            return View(narudzba);
        }

        // POST: Narudzba/Edit/5
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Edit(int id, [Bind("ID,DatumNarudzbe,DatumObrade,Status")] Narudzba narudzba, string korisnikId, string kurirskaSluzbaId, int artikalId)
        {
            if (id != narudzba.ID) return NotFound();

            if (ModelState.IsValid)
            {
                try
                {
                    narudzba.Korisnik = await _context.Users.FindAsync(korisnikId);
                    narudzba.KurirskaSluzba = await _context.Users.FindAsync(kurirskaSluzbaId);
                    narudzba.Artikal = await _context.Artikal.FindAsync(artikalId);

                    _context.Update(narudzba);
                    await _context.SaveChangesAsync();
                }
                catch (DbUpdateConcurrencyException)
                {
                    if (!NarudzbaExists(narudzba.ID)) return NotFound();
                    else throw;
                }
                return RedirectToAction(nameof(Index));
            }

            ViewData["ArtikalID"] = new SelectList(_context.Artikal, "ID", "Naziv", artikalId);
            ViewData["KorisnikID"] = new SelectList(_context.Users, "Id", "Ime", korisnikId);
            ViewData["KurirskaSluzbaID"] = new SelectList(_context.Users, "Id", "Ime", kurirskaSluzbaId);
            return View(narudzba);
        }

        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "KurirskaSluzba, Admin")]
        public async Task<IActionResult> PromijeniStatus(int id, string noviStatus, string returnUrl = null)
        {
            var narudzba = await _context.Narudzba
                .Include(n => n.Korisnik)
                .Include(n => n.Artikal)
                .FirstOrDefaultAsync(n => n.ID == id);

            if (narudzba == null)
                return NotFound();

            // Parse noviStatus to enum
            if (!Enum.TryParse<Status>(noviStatus, out var status))
                return BadRequest();

            narudzba.Status = status;

            // Set DatumObrade if finished
            if (status == Status.Dostavljen)
                narudzba.DatumObrade = DateTime.Now;

            await _context.SaveChangesAsync();

            // Send email notification
            if (narudzba.Korisnik != null)
            {
                string subject = "Status vaše narudžbe je promijenjen";
                string message = status switch
                {
                    Status.UObradi => $"Vaša narudžba za artikal \"{narudzba.Artikal?.Naziv}\" je sada u obradi.",
                    Status.Dostavljen => $"Vaša narudžba za artikal \"{narudzba.Artikal?.Naziv}\" je završena i označena kao dostavljena.",
                    _ => $"Status vaše narudžbe je promijenjen u: {status}."
                };
                await _mailService.SendEmailAsync(narudzba.Korisnik.EmailAdresa, message);
            }

            // Ako je proslijeđen returnUrl, preusmjeri tamo, inače na Index
            if (!string.IsNullOrEmpty(returnUrl) && Url.IsLocalUrl(returnUrl))
            {
                return Redirect(returnUrl);
            }

            return RedirectToAction(nameof(Index));
        }


        // POST: Narudzba/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> DeleteConfirmed(int id)
        {
            var narudzba = await _context.Narudzba.FindAsync(id);
            if (narudzba != null)
            {
                _context.Narudzba.Remove(narudzba);
            }

            await _context.SaveChangesAsync();
            return RedirectToAction(nameof(Index));
        }

        private bool NarudzbaExists(int id)
        {
            return _context.Narudzba.Any(e => e.ID == id);
        }
    }
}