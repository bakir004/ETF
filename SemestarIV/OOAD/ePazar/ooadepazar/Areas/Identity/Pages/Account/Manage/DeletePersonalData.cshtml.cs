// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
#nullable disable

using System;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Logging;
using ooadepazar.Data;
using ooadepazar.Models;

namespace ooadepazar.Areas.Identity.Pages.Account.Manage
{
    public class DeletePersonalDataModel : PageModel
    {
        private readonly UserManager<ApplicationUser> _userManager;
        private readonly SignInManager<ApplicationUser> _signInManager;
        private readonly ILogger<DeletePersonalDataModel> _logger;
        private readonly ApplicationDbContext _context;

        public DeletePersonalDataModel(
            UserManager<ApplicationUser> userManager,
            SignInManager<ApplicationUser> signInManager,
            ILogger<DeletePersonalDataModel> logger,
            ApplicationDbContext context)
        {
            _userManager = userManager;
            _signInManager = signInManager;
            _logger = logger;
            _context = context;
        }

        /// <summary>
        ///     This API supports the ASP.NET Core Identity default UI infrastructure and is not intended to be used
        ///     directly from your code. This API may change or be removed in future releases.
        /// </summary>
        [BindProperty]
        public InputModel Input { get; set; }

        /// <summary>
        ///     This API supports the ASP.NET Core Identity default UI infrastructure and is not intended to be used
        ///     directly from your code. This API may change or be removed in future releases.
        /// </summary>
        public class InputModel
        {
            /// <summary>
            ///     This API supports the ASP.NET Core Identity default UI infrastructure and is not intended to be used
            ///     directly from your code. This API may change or be removed in future releases.
            /// </summary>
            [Required]
            [DataType(DataType.Password)]
            public string Password { get; set; }
        }

        /// <summary>
        ///     This API supports the ASP.NET Core Identity default UI infrastructure and is not intended to be used
        ///     directly from your code. This API may change or be removed in future releases.
        /// </summary>
        public bool RequirePassword { get; set; }

        public async Task<IActionResult> OnGet()
        {
            var user = await _userManager.GetUserAsync(User);
            if (user == null)
            {
                return NotFound($"Unable to load user with ID '{_userManager.GetUserId(User)}'.");
            }

            RequirePassword = await _userManager.HasPasswordAsync(user);
            return Page();
        }

        public async Task<IActionResult> OnPostAsync()
        {
            var user = await _userManager.GetUserAsync(User);
            if (user == null)
            {
                return NotFound($"Unable to load user with ID '{_userManager.GetUserId(User)}'.");
            }

            RequirePassword = await _userManager.HasPasswordAsync(user);
            if (RequirePassword)
            {
                if (!await _userManager.CheckPasswordAsync(user, Input.Password))
                {
                    ModelState.AddModelError(string.Empty, "Incorrect password.");
                    return Page();
                }
            }

            var userId = await _userManager.GetUserIdAsync(user);

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
            if (!result.Succeeded)
            {
                throw new InvalidOperationException($"Unexpected error occurred deleting user.");
            }

            await _signInManager.SignOutAsync();

            _logger.LogInformation("User with ID '{UserId}' deleted themselves.", userId);

            return Redirect("~/");
        }
    }
}