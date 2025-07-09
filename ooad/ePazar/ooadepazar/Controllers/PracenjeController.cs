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
    public class PracenjeController : Controller
    {
        private readonly ApplicationDbContext _context;
        private readonly UserManager<ApplicationUser> _userManager;

        public PracenjeController(ApplicationDbContext context, UserManager<ApplicationUser> userManager)
        {
            _context = context;
            _userManager = userManager;
        }
        
        [Authorize]
        [HttpPost]
        public async Task<IActionResult> Follow(string id) // id of user being followed
        {
            var currentUser = await _userManager.GetUserAsync(User);
            if (currentUser == null || id == currentUser.Id)
                return BadRequest();

            var alreadyFollowing = _context.Pracenje.Any(p =>
                p.PratilacID.Id == currentUser.Id && p.PraceniID.Id == id);

            if (alreadyFollowing)
                return BadRequest("Already following");

            var praceni = await _userManager.FindByIdAsync(id);
            if (praceni == null)
                return NotFound();

            var follow = new Pracenje
            {
                PratilacID = currentUser,
                PraceniID = praceni
            };

            _context.Pracenje.Add(follow);

            var notifikacija = new Notifikacija
            {
                Sadrzaj = $"<a href='/Korisnik/{currentUser.Id}'>{currentUser.Ime} {currentUser.Prezime}</a> vas je zapratio.",
                DatumObjave = DateTime.Now,
                Procitana = false,
                KorisnikId = praceni
            };
            _context.Notifikacija.Add(notifikacija);

            await _context.SaveChangesAsync();

            return RedirectToAction("Index", "Korisnik", new { id });
        }
        
        [Authorize]
        [HttpPost]
        public async Task<IActionResult> Unfollow(string id)
        {
            var currentUser = await _userManager.GetUserAsync(User);
            if (currentUser == null || id == currentUser.Id)
                return BadRequest();

            var relation = _context.Pracenje
                .FirstOrDefault(p => p.PratilacID.Id == currentUser.Id && p.PraceniID.Id == id);

            if (relation != null)
            {
                _context.Pracenje.Remove(relation);
                await _context.SaveChangesAsync();
            }

            return RedirectToAction("Index", "Korisnik", new { id });
        }
        
        
        
        

        // GET: Pracenje
        public async Task<IActionResult> Index()
        {
            return View(await _context.Pracenje.ToListAsync());
        }

        // GET: Pracenje/Details/5
        public async Task<IActionResult> Details(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            var pracenje = await _context.Pracenje
                .FirstOrDefaultAsync(m => m.ID == id);
            if (pracenje == null)
            {
                return NotFound();
            }

            return View(pracenje);
        }

        // GET: Pracenje/Create
        public IActionResult Create()
        {
            return View();
        }

        // POST: Pracenje/Create
        // To protect from overposting attacks, enable the specific properties you want to bind to.
        // For more details, see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Create([Bind("ID")] Pracenje pracenje)
        {
            if (ModelState.IsValid)
            {
                _context.Add(pracenje);
                await _context.SaveChangesAsync();
                return RedirectToAction(nameof(Index));
            }
            return View(pracenje);
        }

        // GET: Pracenje/Edit/5
        public async Task<IActionResult> Edit(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            var pracenje = await _context.Pracenje.FindAsync(id);
            if (pracenje == null)
            {
                return NotFound();
            }
            return View(pracenje);
        }

        // POST: Pracenje/Edit/5
        // To protect from overposting attacks, enable the specific properties you want to bind to.
        // For more details, see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Edit(int id, [Bind("ID")] Pracenje pracenje)
        {
            if (id != pracenje.ID)
            {
                return NotFound();
            }

            if (ModelState.IsValid)
            {
                try
                {
                    _context.Update(pracenje);
                    await _context.SaveChangesAsync();
                }
                catch (DbUpdateConcurrencyException)
                {
                    if (!PracenjeExists(pracenje.ID))
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
            return View(pracenje);
        }

        // GET: Pracenje/Delete/5
        public async Task<IActionResult> Delete(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            var pracenje = await _context.Pracenje
                .FirstOrDefaultAsync(m => m.ID == id);
            if (pracenje == null)
            {
                return NotFound();
            }

            return View(pracenje);
        }

        // POST: Pracenje/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> DeleteConfirmed(int id)
        {
            var pracenje = await _context.Pracenje.FindAsync(id);
            if (pracenje != null)
            {
                _context.Pracenje.Remove(pracenje);
            }

            await _context.SaveChangesAsync();
            return RedirectToAction(nameof(Index));
        }

        private bool PracenjeExists(int id)
        {
            return _context.Pracenje.Any(e => e.ID == id);
        }
    }
}
