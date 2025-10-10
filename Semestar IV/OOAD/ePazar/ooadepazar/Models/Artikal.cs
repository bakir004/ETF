using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace ooadepazar.Models;

public class Artikal
{
    [Key]
    public int ID { get; set; }
    [Required(ErrorMessage = "Naziv je obavezan.")]
    public string Naziv { get; set; }
    [Required(ErrorMessage = "Izaberite Stanje")]
    public Stanje Stanje { get; set; }
    [Required(ErrorMessage = "Opis je obavezan.")]
    public string Opis { get; set; }

    [Required(ErrorMessage = "Cijena je obavezna.")]
    [Range(0, 1000000, ErrorMessage = "Cijena mora biti između 0 i 1,000,000.")]
    public float Cijena { get; set; }
    public String? SlikaUrl { get; set; }
    public Kategorija Kategorija { get; set; }

    [Required(ErrorMessage = "Lokacija je obavezna.")]
    [StringLength(20, ErrorMessage = "Lokacija može imati najviše 20 karaktera.")]
    [RegularExpression("^[a-zA-Z0-9čćžšđČĆŽŠĐ \\-]{1,20}$", ErrorMessage = "Dozvoljena su slova (čćžšđ), brojevi, razmaci i crtice.")]
    public string Lokacija { get; set; }

    public DateTime DatumObjave { get; set; }
    public DateTime DatumAzuriranja { get; set; }
    public Boolean Narucen { get; set; }
    public ApplicationUser? Korisnik { get; set; }
}