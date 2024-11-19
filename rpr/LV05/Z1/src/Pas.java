import java.util.Date;

public class Pas extends Ljubimac implements Objekat{
    private VrstaPsa vrsta;

    public Pas(String ime, Date datumRodjenja, String bolesti, VrstaPsa vrsta) {
        super(ime, datumRodjenja, bolesti);
        this.vrsta = vrsta;
    }

    public VrstaPsa getVrsta() {
        return vrsta;
    }

    public void setVrsta(VrstaPsa vrsta) {
        this.vrsta = vrsta;
    }

    @Override
    public String PrikaziInformacije() {
        return "Pas: " + vrsta;
    }
}
