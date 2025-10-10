namespace ooadepazar.Models.ViewModels;

public class KorisnikArtikliViewModel
{
    public ApplicationUser Korisnik { get; set; }
    public List<Artikal> Artikli { get; set; }
    public List<Narudzba> Narudzbe { get; set; }
}