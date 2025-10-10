package Hrana;

import Interfaces.IZdravlja;

import java.util.List;

public class Voce extends Biljka implements IZdravlja {
    public Voce(String zemljaPorijekla, double koeficijentZdravlja, List<Double> nutritivneVrijednosti, String latinskiNaziv) {
        super(zemljaPorijekla, koeficijentZdravlja, nutritivneVrijednosti, latinskiNaziv);
    }

    @Override
    public Boolean Zdravlje() {
        return getKoeficijentZdravlja() > 0.75 && DajBrojKalorija() < 50;
    }
}
