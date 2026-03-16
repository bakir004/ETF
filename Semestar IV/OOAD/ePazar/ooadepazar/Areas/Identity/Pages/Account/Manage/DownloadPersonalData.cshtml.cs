using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.Extensions.Logging;
using ooadepazar.Models;
using iText.Kernel.Pdf;
using iText.Layout;
using iText.Layout.Element;
using iText.Layout.Properties;
using iText.Kernel.Font;
using iText.IO.Font.Constants;
using iText.Kernel.Colors;

namespace ooadepazar.Areas.Identity.Pages.Account.Manage
{
    public class DownloadPersonalDataModel : PageModel
    {
        private readonly UserManager<ApplicationUser> _userManager;
        private readonly ILogger<DownloadPersonalDataModel> _logger;

        public DownloadPersonalDataModel(
            UserManager<ApplicationUser> userManager,
            ILogger<DownloadPersonalDataModel> logger)
        {
            _userManager = userManager;
            _logger = logger;
        }

        public IActionResult OnGet()
        {
            return NotFound();
        }

        public async Task<IActionResult> OnPostAsync()
        {
            var user = await _userManager.GetUserAsync(User);
            if (user == null)
            {
                return NotFound($"Unable to load user with ID '{_userManager.GetUserId(User)}'.");
            }

            _logger.LogInformation("User with ID '{UserId}' asked for their personal data.", _userManager.GetUserId(User));

            // Kreiraj PDF dokument
            using var memoryStream = new MemoryStream();
            var writer = new PdfWriter(memoryStream);
            var pdf = new PdfDocument(writer);
            var document = new Document(pdf);

            // Kreiraj fontove
            var regularFont = PdfFontFactory.CreateFont(StandardFonts.HELVETICA, "Cp1250");
            var boldFont = PdfFontFactory.CreateFont(StandardFonts.HELVETICA_BOLD, "Cp1250");
            var italicFont = PdfFontFactory.CreateFont(StandardFonts.HELVETICA_OBLIQUE, "Cp1250");

            document.SetFont(regularFont);

            // Naslov dokumenta
            var title = new Paragraph("LICNI PODACI KORISNIKA")
                .SetTextAlignment(TextAlignment.CENTER)
                .SetFont(boldFont)
                .SetFontSize(18)
                .SetMarginBottom(20);
            document.Add(title);

            // Datum kreiranja
            var dateCreated = new Paragraph($"Datum kreiranja: {DateTime.Now:dd.MM.yyyy HH:mm}")
                .SetTextAlignment(TextAlignment.RIGHT)
                .SetFontSize(10)
                .SetMarginBottom(20);
            document.Add(dateCreated);

            // Osnovni podaci korisnika
            document.Add(new Paragraph("OSNOVNI PODACI")
                .SetFont(boldFont)
                .SetFontSize(14)
                .SetMarginBottom(10));

            // Kreiraj tabelu za osnovne podatke
            var table = new Table(2).UseAllAvailableWidth();
            table.SetMarginBottom(20);

            // Dodaj osnovne podatke
            AddTableRow(table, "Ime:", user.Ime ?? "N/A");
            AddTableRow(table, "Prezime:", user.Prezime ?? "N/A");
            AddTableRow(table, "Email:", user.Email ?? "N/A");
            AddTableRow(table, "Korisnicko ime:", user.UserName ?? "N/A");
            AddTableRow(table, "Broj telefona:", user.BrojTelefona ?? "N/A");
            AddTableRow(table, "Adresa:", user.Adresa ?? "N/A");
            AddTableRow(table, "Datum registracije:", user.DatumRegistracije.ToString("dd.MM.yyyy HH:mm"));

            // Kurirska služba - da/ne
            var roles = await _userManager.GetRolesAsync(user);
            bool isKurirskaSluzba = roles.Contains("KurirskaSluzba");

            string kurirskaSluzbaStatus = isKurirskaSluzba ? "DA" : "NE";
            AddTableRow(table, "Kurirska služba:", kurirskaSluzbaStatus);

            string nazivKurirskeSluzbe = isKurirskaSluzba && !string.IsNullOrEmpty(user.KurirskaSluzba)
                ? user.KurirskaSluzba
                : "-";
            if (isKurirskaSluzba && nazivKurirskeSluzbe == "-")
            {
                nazivKurirskeSluzbe = user.Ime + " " + user.Prezime;
            }
            AddTableRow(table, "Naziv kurirske službe:", nazivKurirskeSluzbe);

            document.Add(table);

            // Dodaj dodatne podatke ako postoje
            var personalData = new Dictionary<string, string>();
            var personalDataProps = typeof(ApplicationUser).GetProperties().Where(
                            prop => Attribute.IsDefined(prop, typeof(PersonalDataAttribute)));

            if (personalDataProps.Any())
            {
                document.Add(new Paragraph("DODATNI LICNI PODACI")
                    .SetFont(boldFont)
                    .SetFontSize(14)
                    .SetMarginBottom(10));

                var additionalTable = new Table(2).UseAllAvailableWidth();
                additionalTable.SetMarginBottom(20);

                foreach (var p in personalDataProps)
                {
                    var value = p.GetValue(user)?.ToString() ?? "N/A";
                    AddTableRow(additionalTable, $"{p.Name}:", value);
                    personalData.Add(p.Name, value);
                }

                document.Add(additionalTable);
            }

            // Dodaj informacije o vanjskim prijavama
            var logins = await _userManager.GetLoginsAsync(user);
            if (logins.Any())
            {
                document.Add(new Paragraph("VANJSKE PRIJAVE")
                    .SetFont(boldFont)
                    .SetFontSize(14)
                    .SetMarginBottom(10));

                var loginsTable = new Table(2).UseAllAvailableWidth();
                loginsTable.SetMarginBottom(20);

                foreach (var l in logins)
                {
                    AddTableRow(loginsTable, $"{l.LoginProvider}:", l.ProviderKey);
                }

                document.Add(loginsTable);
            }

            // Dodaj autentifikator ključ ako postoji
            var authenticatorKey = await _userManager.GetAuthenticatorKeyAsync(user);
            if (!string.IsNullOrEmpty(authenticatorKey))
            {
                document.Add(new Paragraph("SIGURNOSNI PODACI")
                    .SetFont(boldFont)
                    .SetFontSize(14)
                    .SetMarginBottom(10));

                var securityTable = new Table(2).UseAllAvailableWidth();
                AddTableRow(securityTable, "Autentifikator kljuc:", authenticatorKey);
                document.Add(securityTable);
            }

            // Dodaj footer
            var footer = new Paragraph("Ovaj dokument sadrži sve vaše licne podatke pohranjene u sistemu.")
                .SetTextAlignment(TextAlignment.CENTER)
                .SetFont(italicFont)
                .SetFontSize(10)
                .SetMarginTop(30);
            document.Add(footer);

            // Zatvori dokument
            document.Close();

            // Vrati PDF kao file download
            var fileName = $"LicniPodaci_{user.UserName}_{DateTime.Now:yyyyMMdd_HHmmss}.pdf";
            return File(memoryStream.ToArray(), "application/pdf", fileName);
        }

        private void AddTableRow(Table table, string label, string value)
        {
            table.AddCell(new Cell().Add(new Paragraph(label).SetFont(PdfFontFactory.CreateFont(StandardFonts.HELVETICA_BOLD))));
            table.AddCell(new Cell().Add(new Paragraph(value)));
        }
    }
}