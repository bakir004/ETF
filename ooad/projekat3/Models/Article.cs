using System.ComponentModel.DataAnnotations;

namespace projekat3.Models;

public class Article
{
    public int Id { get; set; }

    [Required]
    [MaxLength(100)]
    public string Title { get; set; } = string.Empty;

    [Required]
    public string Description { get; set; } = string.Empty;

    [Required]
    [MaxLength(100)]
    public string Location { get; set; } = string.Empty;

    [Required]
    public string ImageBase64 { get; set; } = string.Empty;

    [Required]
    [Range(0, double.MaxValue)]
    public decimal Price { get; set; }

    public List<int> CommentIds { get; set; } = new();

    public Category Category { get; set; }

    [Required]
    public bool IsNew { get; set; }

    public DateTime DateCreated { get; set; } = DateTime.UtcNow;

    public DateTime DateUpdated { get; set; } = DateTime.UtcNow;

    [Required]
    public int CreatorId { get; set; }

    // Navigation property
    public User Creator { get; set; } = null!;
} 