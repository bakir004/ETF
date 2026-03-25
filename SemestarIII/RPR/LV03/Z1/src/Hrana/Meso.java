package Hrana;

import Enums.VrstaMesa;
import Interfaces.IZdravlja;

import java.util.List;

public class Meso extends Hrana implements IZdravlja {
    private VrstaMesa vrstaMesa;

    public Meso(String zemljaPorijekla, double koeficijentZdravlja, List<Double> nutritivneVrijednosti, VrstaMesa vrstaMesa) {
        super(zemljaPorijekla, koeficijentZdravlja, nutritivneVrijednosti);
        this.vrstaMesa = vrstaMesa;
    }

    public VrstaMesa getVrstaMesa() {
        return vrstaMesa;
    }

    public void setVrstaMesa(VrstaMesa vrstaMesa) {
        this.vrstaMesa = vrstaMesa;
    }

    @Override
    public double DajBrojKalorija() {
        return getNutritivneVrijednosti().stream().mapToDouble(Double::doubleValue).sum() * 1.2;
    }

    @Override
    public Boolean Zdravlje() {
        return getKoeficijentZdravlja() > 0.95;
    }
}
