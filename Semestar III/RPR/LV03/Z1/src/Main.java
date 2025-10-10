import Enums.VrstaMesa;
import Hrana.Meso;
import Hrana.Povrce;
import Hrana.Voce;
import Interfaces.IZdravlja;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        Prodavac prodavac = new Prodavac(
                "Bakir",
                "Cinjarevic",
                "1234567890123",
                2401
        );
        Voce v = new Voce(
                "BiH",
                0.8,
                new ArrayList<>(Arrays.asList(1., 2., 3., 4., 5.)),
                "Malus domestica"
        );
        Povrce p = new Povrce(
                "BiH",
                0.6,
                new ArrayList<>(Arrays.asList(5., 6., 7., 8., 9.)),
                "Brassica oleracea"
        );
        Meso m = new Meso(
                "BiH",
                0.9,
                new ArrayList<>(Arrays.asList(12., 13., 14., 15., 16.)),
                VrstaMesa.JANJETINA
        );
        System.out.println("Hrana.Voce: " + v.getKoeficijentZdravlja() + ", " + v.getLatinskiNaziv());
        System.out.println("Hrana.Povrce: " + p.getKoeficijentZdravlja() + ", " + p.getLatinskiNaziv());
        System.out.println("Hrana.Meso: " + m.getKoeficijentZdravlja() + ", " + m.getVrstaMesa());
        List<IZdravlja> lista = new ArrayList<>();
        lista.add(prodavac);
        lista.add(v);
        lista.add(p);
        lista.add(m);
        lista.forEach(x -> System.out.println(x.Zdravlje()));
    }
}