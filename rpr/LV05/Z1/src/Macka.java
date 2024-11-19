import java.util.Date;

public class Macka extends Ljubimac implements Objekat {
    private VrstaMacke vrsta;

    public Macka(String ime, Date datumRodjenja, String bolesti, VrstaMacke vrsta) {
        super(ime, datumRodjenja, bolesti);
        this.vrsta = vrsta;
    }

    public VrstaMacke getVrsta() {
        return vrsta;
    }

    public void setVrsta(VrstaMacke vrsta) {
        this.vrsta = vrsta;
    }

    @Override
    public String PrikaziInformacije() {
        return "Mačka: " + vrsta;
    }
}
