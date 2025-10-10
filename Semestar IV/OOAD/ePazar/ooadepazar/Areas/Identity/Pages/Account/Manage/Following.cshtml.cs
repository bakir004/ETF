using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using ooadepazar.Data;
using ooadepazar.Models;

[Authorize]
public class FollowingModel : PageModel
{
    private readonly UserManager<ApplicationUser> _userManager;
    private readonly ApplicationDbContext _context;

    public FollowingModel(UserManager<ApplicationUser> userManager, ApplicationDbContext context)
    {
        _userManager = userManager;
        _context = context;
    }

    public List<ApplicationUser> Following { get; set; }

    public async Task OnGetAsync()
    {
        var currentUser = await _userManager.GetUserAsync(User);

        Following = await _context.Pracenje
            .Where(p => p.PratilacID.Id == currentUser.Id)
            .Select(p => p.PraceniID)
            .ToListAsync();
    }
    
    public async Task<IActionResult> OnPostRemoveFollowerAsync(string userId)
    {
        var currentUser = await _userManager.GetUserAsync(User);

        var toRemove = await _context.Pracenje
            .FirstOrDefaultAsync(p => p.PratilacID.Id == currentUser.Id && p.PraceniID.Id == userId);

        if (toRemove != null)
        {
            _context.Pracenje.Remove(toRemove);
            await _context.SaveChangesAsync();
        }

        return RedirectToPage(); // Refresh the page
    }
}