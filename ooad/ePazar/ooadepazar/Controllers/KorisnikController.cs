using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using ooadepazar.Data;
using ooadepazar.Models;
using ooadepazar.Models.ViewModels;
using static NuGet.Packaging.PackagingConstants;

namespace ooadepazar.Controllers;

public class KorisnikController : Controller
{
    private readonly UserManager<ApplicationUser> _userManager;
    private readonly ApplicationDbContext _context;

    public KorisnikController(UserManager<ApplicationUser> userManager, ApplicationDbContext context)
    {
        _userManager = userManager;
        _context = context;
    }

    [Route("Korisnik/{id}")]
    public async Task<IActionResult> Index(string id)
    {
        if (string.IsNullOrEmpty(id))
            return NotFound();

        var user = await _userManager.Users.FirstOrDefaultAsync(u => u.Id == id);
        if (user == null)
            return NotFound();

        var artikli = await _context.Artikal
            .Include(a => a.Korisnik) // Include the navigation property
            .Where(a => a.Korisnik.Id == id)
            .ToListAsync();

        var narudzbe = await _context.Narudzba
            .Include(n => n.Artikal) // u?itaj povezani artikal
            .Where(n => n.Korisnik.Id == user.Id)
            .OrderByDescending(n => n.DatumObrade)
            .ToListAsync();

        var viewModel = new KorisnikArtikliViewModel
        {
            Korisnik = user,
            Artikli = artikli,
            Narudzbe = narudzbe
        };

        return View(viewModel);
    }
}