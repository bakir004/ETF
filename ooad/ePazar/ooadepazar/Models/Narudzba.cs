using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace ooadepazar.Models;

public class Narudzba
{
    [Key]
    public int ID { get; set; }

    [Required]
    public DateTime DatumNarudzbe { get; set; }

    public DateTime? DatumObrade { get; set; }

    [Required]  
    public Status Status { get; set; }  // Kreiran, UObradi, Dostavljen

    public String Lokacija { get; set; }

    public ApplicationUser? Korisnik { get; set; }

    public Artikal? Artikal { get; set; }
    
    public ApplicationUser? KurirskaSluzba { get; set; }
    
}