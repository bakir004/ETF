using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using ooadepazar.Data;
using ooadepazar.Models;
using Microsoft.AspNetCore.Mvc.Rendering;
using System.Linq;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Markdig;
using Microsoft.Extensions.Logging;
using Microsoft.AspNetCore.Identity;

namespace ooadepazar.Controllers;

public class HomeController : Controller
{
    private readonly ILogger<HomeController> _logger;
    private readonly ApplicationDbContext _context;
    private readonly UserManager<ApplicationUser> _userManager;

    public HomeController(ILogger<HomeController> logger, ApplicationDbContext context, UserManager<ApplicationUser> userManager)
    {
        _logger = logger;
        _context = context;
        _userManager = userManager;
    }

    // This is now your main e-commerce homepage action
    public async Task<IActionResult> Index(string searchQuery, int? categoryId, string sortBy)
    {
        // --- Prepare Dropdown Options for the View ---

        // Populate Kategorija dropdown options
        var kategorije = Enum.GetValues(typeof(Kategorija))
                             .Cast<Kategorija>()
                             .Select(k => new SelectListItem
                             {
                                 Value = ((int)k).ToString(),
                                 Text = k.ToString().Replace("_", " ")
                             })
                             .OrderBy(x => x.Text)
                             .ToList();

        kategorije.Insert(0, new SelectListItem { Value = "", Text = "Sve kategorije", Selected = true });
        ViewBag.KategorijaOptions = kategorije;

        // Populate Sorting dropdown options
        var sortOptions = new List<SelectListItem>
        {
            new SelectListItem { Value = "latest", Text = "Najnovije" },
            new SelectListItem { Value = "price_asc", Text = "Cijena (rastuće)" },
            new SelectListItem { Value = "price_desc", Text = "Cijena (opadajuće)" },
            new SelectListItem { Value = "name_asc", Text = "Naziv (A-Z)" }
        };
        ViewBag.SortOptions = sortOptions;

        // --- Pass Current Filter/Sort Values Back to the View (for UI state) ---
        ViewBag.CurrentSearchQuery = searchQuery;
        ViewBag.CurrentCategoryId = categoryId;
        ViewBag.CurrentSortBy = sortBy;

        // --- Check if current user is Admin ---
        var currentUser = await _userManager.GetUserAsync(User);
        bool isAdmin = currentUser != null && await _userManager.IsInRoleAsync(currentUser, "Admin");

        // --- Fetch, Filter, and Sort Articles ---

        // Start with all articles and eager load the Korisnik (user) data
        var artikli = _context.Artikal
            .Include(a => a.Korisnik)
            .AsQueryable();

        // Filter by order status based on user role
        if (!isAdmin)
        {
            // Regular users can only see articles that are not ordered
            artikli = artikli.Where(a => a.Narucen == false);
        }
        // Admin can see all articles (both ordered and not ordered)

        // Apply Search Filtering
        if (!string.IsNullOrEmpty(searchQuery))
        {
            artikli = artikli.Where(a => a.Naziv.Contains(searchQuery) || a.Opis.Contains(searchQuery) || a.Lokacija.Contains(searchQuery));
        }

        // Apply Category Filtering
        if (categoryId.HasValue)
        {
            artikli = artikli.Where(a => (int)a.Kategorija == categoryId.Value);
        }

        // Apply Sorting
        switch (sortBy)
        {
            case "price_asc":
                artikli = artikli.OrderBy(a => a.Cijena);
                break;
            case "price_desc":
                artikli = artikli.OrderByDescending(a => a.Cijena);
                break;
            case "name_asc":
                artikli = artikli.OrderBy(a => a.Naziv);
                break;
            case "latest": // Default sorting
            default:
                artikli = artikli.OrderByDescending(a => a.DatumObjave);
                break;
        }

        // --- Execute Query and get articles ---
        var finalArtikli = await artikli.ToListAsync();
        finalArtikli = finalArtikli.Where(item => item != null).ToList();

        // --- Fetch narudžbe for ordered articles ---
        var narudzbe = new Dictionary<int, int>(); // artikalId -> narudzbaId

        var orderedArtikli = finalArtikli.Where(a => a.Narucen).Select(a => a.ID).ToList();
        if (orderedArtikli.Any())
        {
            var narudzbeList = await _context.Narudzba
                .Where(n => orderedArtikli.Contains(n.Artikal.ID))
                .Select(n => new { ArtikalId = n.Artikal.ID, NarudzbaId = n.ID })
                .ToListAsync();

            foreach (var narudzba in narudzbeList)
            {
                narudzbe[narudzba.ArtikalId] = narudzba.NarudzbaId;
            }
        }

        // Pass narudžbe dictionary to the view
        ViewBag.Narudzbe = narudzbe;

        return View(finalArtikli); // Pass the filtered and sorted articles to the view
    }

    public async Task<IActionResult> NE_ULAZITI_NA_OVU_RUTU_ODOSE_PARE()
    {
        return View();
    }

    [HttpGet("Home/GetAIResponseInMarkdown/{artikalId}")]
    public async Task<IActionResult> GetAIResponseInMarkdown(int artikalId)
    {
        var artikal = await _context.Artikal.FindAsync(artikalId);
        if (artikal == null)
        {
            return NotFound($"Artikal with ID {artikalId} not found.");
        }

        string prompt = $"💰{artikal.Naziv}💰\n🏘️ {artikal.Opis}\n\nCIJENA: {artikal.Cijena}, LOKACIJA: {artikal.Lokacija}";
        OpenAIController c = new OpenAIController();
        string markdown = await c.SendMessageAsync(prompt);

        string html = Markdown.ToHtml(markdown);
        return Content(html, "text/html");
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }
}