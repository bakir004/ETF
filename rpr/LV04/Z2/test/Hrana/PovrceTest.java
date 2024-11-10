package Hrana;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class PovrceTest {
    private static Povrce povrce;

    @Test
    void zdravlje() {
        povrce.setKoeficijentZdravlja(0.6);
        povrce.setNutritivneVrijednosti(List.of(1.0, 50.0, 90.0));
        assertFalse(povrce.Zdravlje());
        povrce.setKoeficijentZdravlja(0.9);
        povrce.setNutritivneVrijednosti(List.of(1.0, 5.0, 9.0));
        assertFalse(povrce.Zdravlje());
        povrce.setKoeficijentZdravlja(0.9);
        povrce.setNutritivneVrijednosti(List.of(1.0, 50.0, 90.0));
        assertFalse(povrce.Zdravlje());
        povrce.setKoeficijentZdravlja(0.6);
        povrce.setNutritivneVrijednosti(List.of(1.0, 5.0, 9.0));
        assertTrue(povrce.Zdravlje());
    }

    @BeforeAll
    static void beforeAll() {
        povrce = new Povrce("Bosna", 0.7, List.of(1.0, 2.0, 3.0), "Krompir");
    }
}