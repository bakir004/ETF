using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace ooadepazar.Models;

public class Artikal
{
    [Key]
    public int ID { get; set; }
    [Required]
    public string Naziv { get; set; }
    [Required]
    public Stanje Stanje { get; set; }
    [Required]
    public string Opis { get; set; }
    [Required]
    public float Cijena { get; set; }
    public String? SlikaUrl { get; set; }
    public Kategorija Kategorija { get; set; }
    [Required]
    public string Lokacija { get; set; }
    public DateTime DatumObjave { get; set; }
    public DateTime DatumAzuriranja { get; set; }
    public ApplicationUser? Korisnik { get; set; }
}