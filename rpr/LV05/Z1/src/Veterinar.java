import java.util.ArrayList;
import java.util.List;

public class Veterinar implements Objekat {
    private String ime;
    private Specijalizacija specijalizacija;
    private List<Ljubimac> pregledi;

    public Veterinar(String ime, Specijalizacija specijalizacija) {
        this.ime = ime;
        this.specijalizacija = specijalizacija;
        this.pregledi = new ArrayList<>();
    }

    public String getIme() {
        return ime;
    }
    public void setIme(String ime) {
        this.ime = ime;
    }
    public Specijalizacija getSpecijalizacija() {
        return specijalizacija;
    }
    public void setSpecijalizacija(Specijalizacija specijalizacija) {
        this.specijalizacija = specijalizacija;
    }
    public List<Ljubimac> getPregledi() {
        return pregledi;
    }
    public void setPregledi(List<Ljubimac> pregledi) {
        this.pregledi = pregledi;
    }

    public void PregledajLjubimca(Ljubimac l) throws ValidacijaVrsteException {
        if(!pregledi.isEmpty())
            throw new ValidacijaVrsteException("Samo jedan ljubimac moze biti pregledan");
        pregledi.add(l);
    }

    @Override
    public String PrikaziInformacije() {
        return "Veterinar: " + ime;
    }
}
