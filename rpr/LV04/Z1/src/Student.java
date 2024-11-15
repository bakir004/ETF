import java.util.Date;

public class Student extends Osoba
{
    private String brojIndeksa;
    private Integer godinaStudija;
    private Double prosjek;

    public Student (String ime, String prezime, String adresa, Date datumRodjenja, String brojIndeksa, Integer godinaStudija, Double prosjek)
    {
        super(ime, prezime, adresa, datumRodjenja);
        this.brojIndeksa = brojIndeksa;
        this.godinaStudija = godinaStudija;
        this.prosjek = prosjek;
    }

    public String DajInformacije()
    {
        return "Student: " + ime + " " + prezime + ", broj indeksa: " + brojIndeksa;
    }
}