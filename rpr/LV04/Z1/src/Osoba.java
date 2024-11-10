import java.util.Date;

public abstract class Osoba implements IInformacije
{
    protected String ime, prezime, adresa;
    protected Date datumRodjenja;

    public Osoba (String ime, String prezime, String adresa, Date datumRodjenja)
    {
        this.ime = ime;
        this.prezime = prezime;
        this.adresa = adresa;
        this.datumRodjenja = datumRodjenja;
    }

//    public boolean ProvjeriMaticniBroj()
//    {
//        boolean danIsti = datumRodjenja.getDate() == Integer.parseInt(maticniBroj.substring(0, 2)), mjesecIsti = datumRodjenja.getMonth() + 1 == Integer.parseInt(maticniBroj.substring(2, 4)), godinaIsta = datumRodjenja.getYear() + 900 == Integer.parseInt(maticniBroj.substring(4, 7));
//        return (danIsti && mjesecIsti && godinaIsta);
//    }

    public String DajInformacije()
    {
        return "Ime i prezime: " + ime + " " + prezime;
    }
}