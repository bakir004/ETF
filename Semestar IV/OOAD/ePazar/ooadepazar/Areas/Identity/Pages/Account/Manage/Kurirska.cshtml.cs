using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using ooadepazar.Data;
using ooadepazar.Models;

[Authorize(Roles = "Kurirska_Sluzba, KurirskaSluzba, Admin")]
public class KurirskaModel : PageModel
{
    private readonly UserManager<ApplicationUser> _userManager;
    private readonly ApplicationDbContext _context;

    public KurirskaModel(UserManager<ApplicationUser> userManager, ApplicationDbContext context)
    {
        _userManager = userManager;
        _context = context;
    }

    public List<Narudzba> Orders { get; set; } = new List<Narudzba>();
    public string NazivKurirskeSluzbe { get; set; } = string.Empty;
    public string ImeKorsinika { get; set; } = string.Empty;
    public string PrezimeKorisnika { get; set; } = string.Empty;

    public async Task OnGetAsync()
    {
        var currentUser = await _userManager.GetUserAsync(User);
        if (currentUser != null)
        {
            // Postavi naziv kurirske službe ili ime i prezime korisnika
            NazivKurirskeSluzbe = currentUser.KurirskaSluzba ?? string.Empty;
            ImeKorsinika = currentUser.Ime ?? string.Empty;
            PrezimeKorisnika = currentUser.Prezime ?? string.Empty;

            // Dohvati sve narudžbe dodijeljene trenutnoj kurirskoj službi
            Orders = await _context.Narudzba
                .Include(n => n.Artikal) // učitaj povezani artikal
                .Include(n => n.Korisnik) // učitaj informacije o kupcu
                .Include(n => n.KurirskaSluzba) // učitaj informacije o kurirskoj službi
                .Where(n => n.KurirskaSluzba != null && n.KurirskaSluzba.Id == currentUser.Id)
                .OrderByDescending(n => n.DatumNarudzbe) // sortiraj po datumu narudžbe (najnovije prvo)
                .ThenBy(n => n.Status) // zatim po statusu
                .ToListAsync();
        }
    }

    public async Task<IActionResult> OnPostOznaciKaoDostavljenaAsync(int id)
    {
        var currentUser = await _userManager.GetUserAsync(User);
        if (currentUser == null)
        {
            return RedirectToPage("/Account/Login");
        }

        var narudzba = await _context.Narudzba
            .Include(n => n.KurirskaSluzba)
            .Include(n => n.Artikal)
            .FirstOrDefaultAsync(n => n.ID == id && n.KurirskaSluzba != null && n.KurirskaSluzba.Id == currentUser.Id);

        if (narudzba != null)
        {
            // Provjeri da li je narudžba u odgovarajućem statusu za označavanje kao dostavljenu
            if (narudzba.Status == Status.UObradi)
            {
                narudzba.Status = Status.Dostavljen;
                narudzba.DatumObrade = DateTime.Now; // Ažuriraj datum obrade
                await _context.SaveChangesAsync();
                TempData["SuccessMessage"] = $"Narudžba #{narudzba.ID} je uspješno označena kao dostavljena.";
            }
            else
            {
                TempData["ErrorMessage"] = "Narudžba se ne može označiti kao dostavljena jer nije u statusu 'U obradi'.";
            }
        }
        else
        {
            TempData["ErrorMessage"] = "Narudžba nije pronađena ili nemate dozvolu za ovu akciju.";
        }

        return RedirectToPage();
    }
}