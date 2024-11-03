package Hrana;

import Interfaces.IZdravlja;

import java.util.List;

public class Povrce extends Biljka implements IZdravlja {
    public Povrce(String zemljaPorijekla, double koeficijentZdravlja, List<Double> nutritivneVrijednosti, String latinskiNaziv) {
        super(zemljaPorijekla, koeficijentZdravlja, nutritivneVrijednosti, latinskiNaziv);
    }

    @Override
    public Boolean Zdravlje() {
        return 0.5 < getKoeficijentZdravlja() && getKoeficijentZdravlja() < 0.7 && DajBrojKalorija() < 100;
    }
}
