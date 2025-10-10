package Hrana;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import java.util.List;
import Enums.VrstaMesa;

import static org.junit.jupiter.api.Assertions.*;

class MesoTest {
    private static Meso meso;
    private static Meso mesoMutable;

    @Test
    void getVrstaMesa() {
        assertEquals(VrstaMesa.PILETINA, meso.getVrstaMesa());
    }

    @Test
    void setVrstaMesa() {
        mesoMutable.setVrstaMesa(VrstaMesa.PURETINA);
        assertEquals(VrstaMesa.PURETINA, mesoMutable.getVrstaMesa());
        mesoMutable.setVrstaMesa(VrstaMesa.PILETINA);
    }

    @Test
    void dajBrojKalorija() {
        assertEquals(6.0 * 1.2, meso.DajBrojKalorija());
    }

    @Test
    void zdravlje() {
        assertTrue(meso.Zdravlje());
        mesoMutable.setKoeficijentZdravlja(0.94);
        assertFalse(mesoMutable.Zdravlje());
        mesoMutable.setKoeficijentZdravlja(0.99);
    }

    @BeforeAll
    static void beforeAll() {
        meso = new Meso("Bosna", 0.99, List.of(1.0, 2.0, 3.0), VrstaMesa.PILETINA);
        mesoMutable = new Meso("Bosna", 0.99, List.of(1.0, 2.0, 3.0), VrstaMesa.PILETINA);
    }
}