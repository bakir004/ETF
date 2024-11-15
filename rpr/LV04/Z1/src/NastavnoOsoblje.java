import java.util.Date;

public abstract class NastavnoOsoblje extends Osoba
{
    protected Integer norma, godinaZaposlenja;
    protected String kancelarija;

    public NastavnoOsoblje (String ime, String prezime, String adresa, Date datumRodjenja, Integer norma, Integer godinaZaposlenja, String kancelarija)
    {
        super(ime, prezime, adresa, datumRodjenja);
        this.norma = norma;
        this.godinaZaposlenja = godinaZaposlenja;
        this.kancelarija = kancelarija;
    }
}