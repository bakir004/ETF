package model;

import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.Date;

import static org.junit.jupiter.api.Assertions.*;

class OsobaTest {
    @Test
    public void testKonstruktor() {
        // create date 27. 2. 2004.
        Osoba osoba = new Osoba(3, "Bakir", "Cinjarevic", "Inat kuca", new Date(95, 0, 1), "0101995170001", Uloga.STUDENT);
        assertEquals("Bakir", osoba.getIme());
        assertEquals("Cinjarevic", osoba.getPrezime());
        assertEquals("Inat kuca", osoba.getAdresa());
        assertEquals(new Date(95, 0, 1), osoba.getDatumRodjenja());
        assertEquals("0101995170001", osoba.getMaticniBroj());
        assertEquals(Uloga.STUDENT, osoba.getUloga());
    }

    @Test
    public void ispravnoIme() {
        assertThrows(IllegalArgumentException.class, () -> {
            Osoba osoba = new Osoba(3, "Bakir", "Cinjarevic", "Inat kuca", new Date(95, 0, 1), "0101995170001", Uloga.STUDENT);
            osoba.setIme("A");
        });
        assertThrows(IllegalArgumentException.class, () -> {
            Osoba osoba = new Osoba(3, "A", "Cinjarevic", "Inat kuca", new Date(95, 0, 1), "0101995170001", Uloga.STUDENT);
        });
        assertThrows(IllegalArgumentException.class, () -> {
            Osoba osoba = new Osoba(3, "123......asndioansiodnasindaisnidonasiodnaisondioasndioansoidns", "Cinjarevic", "Inat kuca", new Date(95, 0, 1), "0101995170001", Uloga.STUDENT);
        });
    }

    @Test
    public void duzinaJMBG() {
        assertThrows(IllegalArgumentException.class, () -> {
            Osoba osoba = new Osoba(3, "Bakir", "Cinjarevic", "Inat kuca", new Date(95, 0, 1), "01019951700011", Uloga.STUDENT);
        });
        assertThrows(IllegalArgumentException.class, () -> {
            Osoba osoba = new Osoba(3, "Bakir", "Cinjarevic", "Inat kuca", new Date(95, 0, 1), "010199517000", Uloga.STUDENT);
        });
        assertDoesNotThrow(() -> {
            Osoba osoba = new Osoba(3, "Bakir", "Cinjarevic", "Inat kuca", new Date(95, 0, 1), "0101995170001", Uloga.STUDENT);
        });
    }

    @Test
    public void podudaranJMBGSaDatumomRodjenja() {
        assertThrows(IllegalArgumentException.class, () -> {
            Osoba osoba = new Osoba(3, "Bakir", "Cinjarevic", "Inat kuca", new Date(95, 0, 1), "0102995170001", Uloga.STUDENT);
        });
        assertThrows(IllegalArgumentException.class, () -> {
            Osoba osoba = new Osoba(3, "Bakir", "Cinjarevic", "Inat kuca", new Date(95, 0, 1), "0201995170001", Uloga.STUDENT);
        });
        assertThrows(IllegalArgumentException.class, () -> {
            Osoba osoba = new Osoba(3, "Bakir", "Cinjarevic", "Inat kuca", new Date(95, 0, 1), "0101996170001", Uloga.STUDENT);
        });
        assertDoesNotThrow(() -> {
            Osoba osoba = new Osoba(3, "Bakir", "Cinjarevic", "Inat kuca", new Date(95, 0, 1), "0101995170001", Uloga.STUDENT);
        });
    }
    @Test
    public void nepravilanPath() {
        assertThrows(IOException.class, () -> {
            Osoba osoba = new Osoba(3, "Bakir", "Cinjarevic", "Inat kuca", new Date(95, 0, 1), "0101995170001", Uloga.STUDENT);
            Osoba.ucitajOsobeIzTxtDatoteke("src/data/asdasdasd.txt");
        });
    }
}