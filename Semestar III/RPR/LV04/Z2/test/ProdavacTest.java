import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

public class ProdavacTest {
    private static Prodavac prodavac;

    @Test
    void getIme() {
        assertEquals("Nedim", prodavac.getIme());
    }

    @Test
    void setIme() {
        prodavac.setIme("Nedim2");
        assertEquals("Nedim2", prodavac.getIme());
    }

    @Test
    void getPrezime() {
        assertEquals("Sladic", prodavac.getPrezime());
    }

    @Test
    void setPrezime() {
        prodavac.setPrezime("Sladic2");
        assertEquals("Sladic2", prodavac.getPrezime());
        prodavac.setPrezime("Sladic");
    }

    @Test
    void getBrojStanda() {
        assertEquals("45612780", prodavac.getBrojStanda());
    }

    @Test
    void setBrojStanda() {
        prodavac.setBrojStanda("456127802");
        assertEquals("456127802", prodavac.getBrojStanda());
    }

    @Test
    void getID() {
        assertEquals(101, prodavac.getID());
    }

    @Test
    void setID() {
        prodavac.setID(102);
        assertEquals(102, prodavac.getID());
        prodavac.setID(101);
    }

    @Test
    void zdravlje() {
        assertTrue(prodavac.Zdravlje());
        prodavac.setID(102);
        assertFalse(prodavac.Zdravlje());
        prodavac.setID(101);
    }

    @BeforeAll
    static void beforeAll() {
        prodavac = new Prodavac("Nedim", "Sladic", "45612780", 101);
    }
}