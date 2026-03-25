using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using ooadepazar.Data;
using ooadepazar.Models;

[Authorize]
public class OrdersModel : PageModel
{
    private readonly UserManager<ApplicationUser> _userManager;
    private readonly ApplicationDbContext _context;

    public OrdersModel(UserManager<ApplicationUser> userManager, ApplicationDbContext context)
    {
        _userManager = userManager;
        _context = context;
    }

    public List<Narudzba> Orders { get; set; }

    public async Task OnGetAsync()
    {
        var currentUser = await _userManager.GetUserAsync(User);

        Orders = await _context.Narudzba
            .Include(n => n.Artikal) // učitaj povezani artikal
            .Where(n => n.Korisnik.Id == currentUser.Id)
            .OrderByDescending(n => n.DatumObrade)
            .ToListAsync();
    }

    public async Task<IActionResult> OnPostOtkaziNarudzbuAsync(int id)
    {
        var currentUser = await _userManager.GetUserAsync(User);

        var narudzba = await _context.Narudzba
            .Include(n => n.Korisnik)
            .Include(n => n.Artikal)
            .FirstOrDefaultAsync(n => n.ID == id && n.Korisnik.Id == currentUser.Id);

        if (narudzba != null)
        {
            // Postavi artikal kao nenaručen
            if (narudzba.Artikal != null)
            {
                narudzba.Artikal.Narucen = false;
            }

            // Obriši narudžbu
            _context.Narudzba.Remove(narudzba);

            await _context.SaveChangesAsync();
        }

        return RedirectToPage();
    }
}