package Hrana;

import java.util.List;

public abstract class Biljka extends Hrana {
    private String latinskiNaziv;

    public Biljka(String zemljaPorijekla, double koeficijentZdravlja, List<Double> nutritivneVrijednosti, String latinskiNaziv) {
        super(zemljaPorijekla, koeficijentZdravlja, nutritivneVrijednosti);
        this.latinskiNaziv = latinskiNaziv;
    }

    public String getLatinskiNaziv() {
        return latinskiNaziv;
    }

    public void setLatinskiNaziv(String latinskiNaziv) {
        this.latinskiNaziv = latinskiNaziv;
    }
}
