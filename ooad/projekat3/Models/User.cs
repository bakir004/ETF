using System.ComponentModel.DataAnnotations;

namespace projekat3.Models;

public class User
{
    public int Id { get; set; }

    [Required]
    [MaxLength(50)]
    public string FirstName { get; set; } = string.Empty;

    [Required]
    [MaxLength(50)]
    public string LastName { get; set; } = string.Empty;

    [Required]
    [MaxLength(100)]
    public string Location { get; set; } = string.Empty;

    public List<int> Reviews { get; set; } = new();

    [Required]
    public UserRole Role { get; set; }

    // Navigation property for articles created by this user
    public List<Article> Articles { get; set; } = new();
} 