using Microsoft.EntityFrameworkCore;
using projekat3.Models;

namespace projekat3.Data;

public class ApplicationDbContext : DbContext
{
    public ApplicationDbContext(DbContextOptions<ApplicationDbContext> options)
        : base(options)
    {
    }

    public DbSet<User> Users { get; set; }
    public DbSet<Article> Articles { get; set; }

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        base.OnModelCreating(modelBuilder);

        modelBuilder.Entity<Article>()
            .HasOne(a => a.Creator)
            .WithMany(u => u.Articles)
            .HasForeignKey(a => a.CreatorId)
            .OnDelete(DeleteBehavior.Cascade);
    }
} 