using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using ooadepazar.Data;
using ooadepazar.Models;

namespace ooadepazar.Controllers
{
    [Authorize(Roles = "Admin")]
    public class UserManagementController : Controller
    {
        private readonly UserManager<ApplicationUser> _userManager;
        private readonly RoleManager<IdentityRole> _roleManager;
        private readonly ApplicationDbContext _context;

        public UserManagementController(
            UserManager<ApplicationUser> userManager,
            RoleManager<IdentityRole> roleManager,
            ApplicationDbContext context)
        {
            _userManager = userManager;
            _roleManager = roleManager;
            _context = context;
        }

        // GET: /UserManagement
        public async Task<IActionResult> Index()
        {
            var users = await _userManager.Users.ToListAsync();
            var userRolesViewModel = new List<UserRolesViewModel>();

            foreach (var user in users)
            {
                var roles = await _userManager.GetRolesAsync(user);
                userRolesViewModel.Add(new UserRolesViewModel
                {
                    UserId = user.Id,
                    Email = user.Email,
                    Roles = roles
                });
            }

            userRolesViewModel = userRolesViewModel.OrderBy(u => u.Email).ToList();

            return View(userRolesViewModel);
        }

        // GET: /UserManagement/EditRoles/userId
        [HttpGet]
        public async Task<IActionResult> EditRoles(string userId)
        {
            var user = await _userManager.FindByIdAsync(userId);
            if (user == null) return NotFound();

            var allRoles = await _roleManager.Roles.ToListAsync(); // Prevent open DataReader issue
            var userRoles = await _userManager.GetRolesAsync(user); // Safe async call

            var model = new EditUserRolesViewModel
            {
                UserId = user.Id,
                Email = user.Email,
                RoleSelections = allRoles.Select(role => new RoleSelectionViewModel
                {
                    RoleName = role.Name,
                    IsSelected = userRoles.Contains(role.Name)
                }).ToList()
            };

            return View(model);
        }


        // POST: /UserManagement/EditRoles/userId
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> EditRoles(EditUserRolesViewModel model)
        {
            var user = await _userManager.FindByIdAsync(model.UserId);

            if (user == null)
                return NotFound();

            var currentRoles = await _userManager.GetRolesAsync(user);
            var selectedRoles = model.RoleSelections.Where(r => r.IsSelected).Select(r => r.RoleName);

            var rolesToAdd = selectedRoles.Except(currentRoles);
            var rolesToRemove = currentRoles.Except(selectedRoles);

            if (rolesToRemove.Contains("KurirskaSluzba"))
            {
                var narudzbe = await _context.Narudzba
                    .Where(n => n.KurirskaSluzba != null && n.KurirskaSluzba.Id == user.Id)
                    .ToListAsync();

                var defaultKurir = await _userManager.FindByIdAsync("7497c315-522f-431a-a214-0cc3827407ad");
                foreach (var narudzba in narudzbe)
                {
                    narudzba.KurirskaSluzba = defaultKurir;
                }
                await _context.SaveChangesAsync();
            }

            await _userManager.RemoveFromRolesAsync(user, rolesToRemove);
            await _userManager.AddToRolesAsync(user, rolesToAdd);

            return RedirectToAction(nameof(Index));
        }

        // POST: /UserManagement/DeleteUser/userId
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> DeleteUser(string userId)
        {
            var user = await _userManager.FindByIdAsync(userId);

            if (user == null)
                return NotFound();

            // Provjeri da li je korisnik kurir i prebaci narudžbe na default kurira
            var userRoles = await _userManager.GetRolesAsync(user);
            if (userRoles.Contains("KurirskaSluzba"))
            {
                var narudzbe = await _context.Narudzba
                    .Where(n => n.KurirskaSluzba != null && n.KurirskaSluzba.Id == user.Id)
                    .ToListAsync();

                var defaultKurir = await _userManager.FindByIdAsync("7497c315-522f-431a-a214-0cc3827407ad");
                if (defaultKurir != null)
                {
                    foreach (var narudzba in narudzbe)
                    {
                        narudzba.KurirskaSluzba = defaultKurir;
                    }
                    await _context.SaveChangesAsync();
                }
            }

            // 1. Dohvati sve artikle koje je korisnik kreirao
            var korisnikoviArtikli = await _context.Artikal
                .Where(a => a.Korisnik != null && a.Korisnik.Id == user.Id)
                .ToListAsync();

            // 2. Dohvati sve narudžbe za artikle koje je korisnik kreirao
            var narudzbeZaKorisnikoveArtikle = await _context.Narudzba
                .Where(n => n.Artikal != null && korisnikoviArtikli.Select(a => a.ID).Contains(n.Artikal.ID))
                .Include(n => n.Artikal)
                .ToListAsync();

            // 3. Dohvati sve narudžbe koje je korisnik napravio
            var korisnikoveNarudzbe = await _context.Narudzba
                .Where(n => n.Korisnik != null && n.Korisnik.Id == user.Id)
                .Include(n => n.Artikal)
                .ToListAsync();

            // 4. Dohvati sve notifikacije korisnika
            var korisnikoveNotifikacije = await _context.Notifikacija
                .Where(n => n.KorisnikId != null && n.KorisnikId.Id == user.Id)
                .ToListAsync();

            // 5. Resetuj artikle koje je korisnik naručio (postaviti Narucen = false)
            foreach (var narudzba in korisnikoveNarudzbe)
            {
                if (narudzba.Artikal != null)
                {
                    narudzba.Artikal.Narucen = false;
                }
            }

            // 6. PRVI - Obriši sve narudžbe za artikle koje je korisnik kreirao
            _context.Narudzba.RemoveRange(narudzbeZaKorisnikoveArtikle);

            // 7. DRUGI - Obriši narudžbe koje je korisnik napravio
            _context.Narudzba.RemoveRange(korisnikoveNarudzbe);

            // 8. TREĆI - Obriši notifikacije korisnika
            _context.Notifikacija.RemoveRange(korisnikoveNotifikacije);

            // 9. ČETVRTI - Obriši artikle koje je korisnik kreirao
            _context.Artikal.RemoveRange(korisnikoviArtikli);

            // 10. Sačuvaj promjene
            await _context.SaveChangesAsync();

            // Obriši korisnika
            var result = await _userManager.DeleteAsync(user);

            if (result.Succeeded)
            {
                var ukupnoNarudzbi = narudzbeZaKorisnikoveArtikle.Count + korisnikoveNarudzbe.Count;
                var artikliCount = korisnikoviArtikli.Count;
                var notifikacijeCount = korisnikoveNotifikacije.Count;
                var successMessage = $"Korisnik {user.Email} je uspješno obrisan.";
                if (ukupnoNarudzbi > 0)
                {
                    successMessage += $" Obrisano je {ukupnoNarudzbi} narudžbi.";
                }
                if (artikliCount > 0)
                {
                    successMessage += $" Obrisano je {artikliCount} kreiranih artikala.";
                }
                if (notifikacijeCount > 0)
                {
                    successMessage += $" Obrisano je {notifikacijeCount} notifikacija.";
                }
                TempData["Success"] = successMessage;
            }
            else
            {
                TempData["Error"] = "Greška pri brisanju korisnika: " + string.Join(", ", result.Errors.Select(e => e.Description));
            }

            return RedirectToAction(nameof(Index));
        }
    }

    public class UserRolesViewModel
    {
        public string UserId { get; set; }
        public string Email { get; set; }
        public IList<string> Roles { get; set; }
    }

    public class EditUserRolesViewModel
    {
        public string UserId { get; set; }
        public string Email { get; set; }
        public List<RoleSelectionViewModel> RoleSelections { get; set; }
    }

    public class RoleSelectionViewModel
    {
        public string RoleName { get; set; }
        public bool IsSelected { get; set; }
    }
}
