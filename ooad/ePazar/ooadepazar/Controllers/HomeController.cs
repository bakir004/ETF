using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using ooadepazar.Data;
using ooadepazar.Models;
using Microsoft.AspNetCore.Mvc.Rendering; // Required for SelectList and SelectListItem
using System.Linq; // Required for LINQ queries
using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Markdig;
using Microsoft.Extensions.Logging; // Required for Enum

namespace ooadepazar.Controllers;

public class HomeController : Controller
{
    private readonly ILogger<HomeController> _logger;
    private readonly ApplicationDbContext _context;

    public HomeController(ILogger<HomeController> logger, ApplicationDbContext context)
    {
        _logger = logger;
        _context = context; 
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
                                 Text = k.ToString().Replace("_", " ") // Make enum names more readable
                             })
                             .ToList();
        kategorije.Insert(0, new SelectListItem { Value = "", Text = "Sve kategorije", Selected = true }); // Add "All Categories" option
        ViewBag.KategorijaOptions = kategorije;

        // Populate Sorting dropdown options
        var sortOptions = new List<SelectListItem>
        {
            new SelectListItem { Value = "latest", Text = "Najnovije objavljeno" },
            new SelectListItem { Value = "price_asc", Text = "Cijena (rastuće)" },
            new SelectListItem { Value = "price_desc", Text = "Cijena (opadajuće)" },
            new SelectListItem { Value = "name_asc", Text = "Naziv (A-Z)" }
        };
        ViewBag.SortOptions = sortOptions;

        // --- Pass Current Filter/Sort Values Back to the View (for UI state) ---
        ViewBag.CurrentSearchQuery = searchQuery;
        ViewBag.CurrentCategoryId = categoryId;
        ViewBag.CurrentSortBy = sortBy;

        // --- Fetch, Filter, and Sort Articles ---

        // Start with all articles and eager load the Korisnik (user) data
        var artikli = _context.Artikal.Include(a => a.Korisnik).AsQueryable();

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

        // --- Execute Query and Return to View ---

        // Convert to list and apply defensive null check for any potentially null items within the collection
        var finalArtikli = await artikli.ToListAsync();
        finalArtikli = finalArtikli.Where(item => item != null).ToList();

        return View(finalArtikli); // Pass the filtered and sorted articles to the view
    }

    public async Task<IActionResult> NE_ULAZITI_NA_OVU_RUTU_ODOSE_PARE()
    {
        return View();
    }
    
    [HttpGet]
    public async Task<IActionResult> GetAIResponseInMarkdown()
    {
        string prompt = "💰OSUNČAN MANJI STAN💰\n🏘️ KUPITE STAN NA URIJAMA (PRIJEDOR)!!!\n\n🔔ODLIČNO ORGANIZOVAN PROSTOR🔔\nPROJEKTOVANA POVRŠINA: 42,85m2\nSTRUKTURA:\n* kuhinja\n* dnevni boravak sa trpezarijom\n* soba 1\n* kupatilo\n* hodnik\n* lođa\n\nCIJENA: 2.500 KM/m2\nUKUPNA CIJENA: 107.125KM\n\nStan je na četvrtom spratu a zgrada ima šest spratova.\n\nObjekat  posjeduje  dva lifta!\nUz stan se može kupiti i parking mjesto u podzemnoj garaži koje je 1200KM/m2\nVanjska parking mjesta se ne plaćaju.\nGrijanje -podno na struju\n\nKeramika u hodniku, kupatilu i kuhinji!\nParket u sobi, dnevnom boravku i trpezariji.\nStolarija sedmokomorna!";

        OpenAIController c = new OpenAIController();
        // string markdown = await c.SendMessageAsync(prompt);
        string markdown =
            "**Pozitivne strane o stanu:**\n\n1. **Odlično organizovan prostor:** Površina od 42,85m² je dobro iskorištena sa funkcionalnim rasporedom prostorija.\n2. **Dva lifta u zgradi:** Olakšava pristup stanovima na višim spratovima, što je korisno za svakodnevni život i transport materijala.\n3. **Podno grijanje:** Osigurava ravnomjerno grijanje stana, što poboljšava udobnost tokom zimskih mjeseci.\n4. **Kvalitetna stolarija i podne obloge:** Sedmokomorna stolarija i keramičke pločice, kao i parket, doprinose boljem estetskom dojmu i energetskoj efikasnosti.\n\n**Loše strane o stanu:**\n\n1. **Visoka cijena po kvadratnom metru:** Cijena od 2.500 KM/m² je relativno visoka za Prijedor, što može predstavljati izazov za potencijalne kupce.\n2. **Grijanje na struju:** Može rezultirati visokim troškovima tokom zimskih mjeseci ukoliko nije adekvatno izolirano.\n3. **Lokacija na četvrtom spratu:** Iako zgrada ima liftove, neki bi kupci mogli preferirati niže spratove zbog prijatnosti pristupa ili straha od visine.\n\n**Poređenje sa sličnim artiklima na tržištu:**\n\nNa tržištu nekretnina u Prijedoru, cijena od 2.500 KM/m² je iznad prosjeka. Većina stambenih jedinica u ovom gradu kreće se između 1.500 i 2.000 KM po kvadratnom metru, ovisno o lokaciji, kvaliteti gradnje i dodatnim pogodnostima. Međutim, postojanje podzemne garaže i visoki standardi unutrašnjih radova mogu opravdati višu cijenu za neke kupce.\n\n**Zaključak:**\n\nAko tražite funkcionalno organizovan stan s modernim sadržajima i ne smeta vam cijena iznad proseka, ovo bi mogla biti dobra investicija. Ipak, trebali biste pažljivo razmotriti svoju financijsku situaciju i potencijalne dodatne troškove grijanja. Također, bilo bi korisno istražiti slične ponude u Prijedoru kako biste bili sigurni da donosite najbolju odluku.";
        await Task.Delay(5000);
            
        string html = Markdown.ToHtml(markdown);
        return Content(html, "text/html");
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }
}