package Hrana;

import java.util.List;

public abstract class Hrana {
    private String zemljaPorijekla;
    private double koeficijentZdravlja;
    private List<Double> nutritivneVrijednosti;
    public abstract Boolean Zdravlje();
    public double DajBrojKalorija() {
        return nutritivneVrijednosti.stream().mapToDouble(Double::doubleValue).sum();
    }

    public Hrana(String zemljaPorijekla, double koeficijentZdravlja, List<Double> nutritivneVrijednosti) {
        this.zemljaPorijekla = zemljaPorijekla;
        this.koeficijentZdravlja = koeficijentZdravlja;
        this.nutritivneVrijednosti = nutritivneVrijednosti;
    }

    public double getKoeficijentZdravlja() {
        return koeficijentZdravlja;
    }

    public void setKoeficijentZdravlja(double koeficijentZdravlja) {
        this.koeficijentZdravlja = koeficijentZdravlja;
    }

    public String getZemljaPorijekla() {
        return zemljaPorijekla;
    }

    public void setZemljaPorijekla(String zemljaPorijekla) {
        this.zemljaPorijekla = zemljaPorijekla;
    }

    public List<Double> getNutritivneVrijednosti() {
        return nutritivneVrijednosti;
    }

    public void setNutritivneVrijednosti(List<Double> nutritivneVrijednosti) {
        this.nutritivneVrijednosti = nutritivneVrijednosti;
    }
}
