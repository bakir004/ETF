// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
using System;
using System.ComponentModel.DataAnnotations;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.Extensions.Logging;
using ooadepazar.Models;

namespace ooadepazar.Areas.Identity.Pages.Account.Manage
{
    public class PersonalDataModel : PageModel
    {
        private readonly UserManager<ApplicationUser> _userManager;
        private readonly ILogger<PersonalDataModel> _logger;

        public PersonalDataModel(
            UserManager<ApplicationUser> userManager,
            ILogger<PersonalDataModel> logger)
        {
            _userManager = userManager;
            _logger = logger;
        }

        [BindProperty]
        public InputModel Input { get; set; }

        public class InputModel
        {
            [Display(Name = "Ime")]
            public string FirstName { get; set; }

            [Display(Name = "Prezime")]
            public string LastName { get; set; }

            [Display(Name = "Email")]
            public string Email { get; set; }

            [Display(Name = "Broj telefona")]
            [Phone]
            public string PhoneNumber { get; set; }

            [Display(Name = "Lokacija")]
            public string Location { get; set; }
        }

        public async Task<IActionResult> OnGetAsync()
        {
            var user = await _userManager.GetUserAsync(User);
            if (user == null)
            {
                return NotFound($"Unable to load user with ID '{_userManager.GetUserId(User)}'.");
            }

            Input = new InputModel
            {
                FirstName = user.Ime,
                LastName = user.Prezime,
                Email = user.Email,
                PhoneNumber = user.BrojTelefona,
                Location = user.Adresa
            };

            return Page();
        }

        public async Task<IActionResult> OnPostUpdateProfileAsync()
        {
            var user = await _userManager.GetUserAsync(User);
            if (user == null)
            {
                return NotFound($"Unable to load user with ID '{_userManager.GetUserId(User)}'.");
            }

            if (!ModelState.IsValid)
            {
                // Ponovo učitaj podatke ako validacija nije uspješna
                Input.FirstName = user.Ime;
                Input.LastName = user.Prezime;
                Input.Email = user.Email;
                return Page();
            }

            // Ažuriraj samo polja koja mogu biti mijenjana
            if (Input.PhoneNumber != user.BrojTelefona)
            {
                user.BrojTelefona = Input.PhoneNumber;
                var updateResult = await _userManager.UpdateAsync(user);
                if (!updateResult.Succeeded)
                {
                    foreach (var error in updateResult.Errors)
                    {
                        ModelState.AddModelError(string.Empty, error.Description);
                    }
                    return Page();
                }
            }

            if (Input.Location != user.Adresa)
            {
                user.Adresa = Input.Location;
                var updateResult = await _userManager.UpdateAsync(user);
                if (!updateResult.Succeeded)
                {
                    foreach (var error in updateResult.Errors)
                    {
                        ModelState.AddModelError(string.Empty, error.Description);
                    }
                    return Page();
                }
            }

            TempData["StatusMessage"] = "Podaci su uspješno ažurirani.";
            return RedirectToPage();
        }
    }
}