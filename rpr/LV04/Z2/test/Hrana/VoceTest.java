package Hrana;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class VoceTest {
    private static Voce voce;

    @Test
    void zdravlje() {
        voce.setKoeficijentZdravlja(0.94);
        voce.setNutritivneVrijednosti(List.of(1.0, 50.0, 90.0));
        assertFalse(voce.Zdravlje());
        voce.setKoeficijentZdravlja(0.6);
        voce.setNutritivneVrijednosti(List.of(1.0, 50.0, 90.0));
        assertFalse(voce.Zdravlje());
        voce.setKoeficijentZdravlja(0.6);
        voce.setNutritivneVrijednosti(List.of(1.0, 5.0, 9.0));
        assertFalse(voce.Zdravlje());
        voce.setKoeficijentZdravlja(0.9);
        voce.setNutritivneVrijednosti(List.of(1.0, 5.0, 9.0));
        assertTrue(voce.Zdravlje());
    }

    @BeforeAll
    static void beforeAll() {
        voce = new Voce("Bosna", 0.7, List.of(1.0, 2.0, 3.0), "Jabuka");
    }
}