import java.util.Date;

public class Profesor extends NastavnoOsoblje
{
    private String zvanje;
    private Integer brojDiplomanata;

    public Profesor (String ime, String prezime, String adresa, Date datumRodjenja, Integer norma, Integer godinaZaposlenja, String kancelarija, String zvanje, Integer brojDiplomanata)
    {
        super(ime, prezime, adresa, datumRodjenja, norma, godinaZaposlenja, kancelarija);
        this.zvanje = zvanje;
        this.brojDiplomanata = brojDiplomanata;
    }

    public String DajInformacije ()
    {
        return "Profesor: " + zvanje + " " + ime + " " + prezime;
    }
}