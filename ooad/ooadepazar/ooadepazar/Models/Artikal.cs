using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Runtime.InteropServices.JavaScript;

namespace ooadepazar.Models;

public class Artikal
{
    [Key]
    public int ID { get; set; }
    
    public string Naziv { get; set; }
    public Stanje St { get; set; }
    public string Opis { get; set; }
    public float Cijena { get; set; }
    public string Lokacija { get; set; }
    public DateTime DatumObjave { get; set; }
    
    // [ForeignKey("Korisnik")]
    // public int KorisnikId { get; set; }
}