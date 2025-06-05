using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Rendering;
using Microsoft.EntityFrameworkCore;
using ooadepazar.Data;
using ooadepazar.Models;

namespace ooadepazar.Controllers
{
    public class ArtikalController : Controller
    {
        private readonly ApplicationDbContext _context;
        private readonly UserManager<ApplicationUser> _userManager;

        public ArtikalController(ApplicationDbContext context, UserManager<ApplicationUser> userManager)
        {
            _context = context;
            _userManager = userManager;
        }

        // GET: Artikal
        public async Task<IActionResult> Index()
        {
            return View(await _context.Artikal.ToListAsync());
        }

        // GET: Artikal/Details/5
        public async Task<IActionResult> Details(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            var artikal = await _context.Artikal
                .Include(a => a.Korisnik)
                .FirstOrDefaultAsync(m => m.ID == id);
            if (artikal == null)
            {
                return NotFound();
            }

            return View(artikal);
        }

        // GET: Artikal/Create
        public IActionResult Create()
        {
            ViewBag.StanjeOptions = new SelectList(Enum.GetValues(typeof(Stanje)));
            ViewBag.KategorijaOptions = new SelectList(Enum.GetValues(typeof(Kategorija)));
            return View();
        }

        // POST: Artikal/Create
        // To protect from overposting attacks, enable the specific properties you want to bind to.
        // For more details, see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Create([Bind("ID,Naziv,Stanje,Opis,Cijena,Lokacija,DatumObjave,DatumAzuriranja,Kategorija,SlikaUrl")] Artikal artikal)
        {
            ViewBag.StanjeOptions = new SelectList(Enum.GetValues(typeof(Stanje)));
            ViewBag.KategorijaOptions = new SelectList(Enum.GetValues(typeof(Kategorija)));

            artikal.DatumObjave = DateTime.Now;
            artikal.DatumAzuriranja = DateTime.Now;
            if (ModelState.IsValid)
            {
                var currentUser = await _userManager.GetUserAsync(User);

                if (currentUser == null)
                {
                    // Handle case where user is not logged in (e.g., redirect to login)
                    // This scenario should ideally be prevented by [Authorize] attribute
                    return RedirectToPage("/Account/Login", new { area = "Identity" });
                }
                artikal.Korisnik = currentUser;
                
                _context.Add(artikal);
                await _context.SaveChangesAsync();
                return RedirectToAction(nameof(Index));
            }
            return View(artikal);
        }

        // GET: Artikal/Edit/5
        public async Task<IActionResult> Edit(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            var artikal = await _context.Artikal.FindAsync(id);
            if (artikal == null)
            {
                return NotFound();
            }

            ViewBag.StanjeOptions = new SelectList(Enum.GetValues(typeof(Stanje)));
            ViewBag.KategorijaOptions = new SelectList(Enum.GetValues(typeof(Kategorija)));

            return View(artikal);
        }

        // POST: Artikal/Edit/5
        // To protect from overposting attacks, enable the specific properties you want to bind to.
        // For more details, see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Edit(int id, [Bind("ID,Naziv,Stanje,Opis,Cijena,Lokacija,DatumObjave,DatumAzuriranja,Kategorija,SlikaUrl")] Artikal artikal)
        {
            if (id != artikal.ID)
            {
                return NotFound();
            }

            if (ModelState.IsValid)
            {
                try {
                    var existingArtikal = await _context.Artikal.FindAsync(id);
                    if (existingArtikal == null)
                        return NotFound();

                    existingArtikal.Naziv = artikal.Naziv;
                    existingArtikal.Stanje = artikal.Stanje;
                    existingArtikal.Opis = artikal.Opis;
                    existingArtikal.Cijena = artikal.Cijena;
                    existingArtikal.Lokacija = artikal.Lokacija;
                    existingArtikal.Kategorija = artikal.Kategorija;
                    existingArtikal.SlikaUrl = artikal.SlikaUrl;
                    // Do NOT update DatumObjave
                    existingArtikal.DatumAzuriranja = DateTime.Now; // Set to current date/time

                    _context.Update(existingArtikal);
                    await _context.SaveChangesAsync();
                }
                catch (DbUpdateConcurrencyException)
                {
                    if (!ArtikalExists(artikal.ID))
                    {
                        return NotFound();
                    }
                    else
                    {
                        throw;
                    }
                }
                return RedirectToAction(nameof(Index));
            }

            ViewBag.StanjeOptions = new SelectList(Enum.GetValues(typeof(Stanje)));
            ViewBag.KategorijaOptions = new SelectList(Enum.GetValues(typeof(Kategorija)));

            return View(artikal);
        }

        // GET: Artikal/Delete/5
        public async Task<IActionResult> Delete(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            var artikal = await _context.Artikal
                .FirstOrDefaultAsync(m => m.ID == id);
            if (artikal == null)
            {
                return NotFound();
            }

            return View(artikal);
        }

        // POST: Artikal/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> DeleteConfirmed(int id)
        {
            var artikal = await _context.Artikal.FindAsync(id);
            if (artikal != null)
            {
                _context.Artikal.Remove(artikal);
            }

            await _context.SaveChangesAsync();
            return RedirectToAction(nameof(Index));
        }

        private bool ArtikalExists(int id)
        {
            return _context.Artikal.Any(e => e.ID == id);
        }
    }
}
