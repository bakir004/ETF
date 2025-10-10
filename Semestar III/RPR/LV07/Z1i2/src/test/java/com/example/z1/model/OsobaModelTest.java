package com.example.z1.model;

import javafx.collections.ObservableList;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

import static org.junit.jupiter.api.Assertions.*;

class OsobaModelTest {
    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
    @Test
    void nePostojiDatoteka() {
        OsobaModel om = new OsobaModel();
        assertThrows(Exception.class, () -> om.napuniPodatkeIzTxtDatoteke("src/data/nepostojecaOsoba.txt"));
    }

    @Test
    void neispravanFormatDatoteke() throws IOException, ParseException {
        OsobaModel om = new OsobaModel();
        om.napuniPodatkeIzTxtDatoteke("src/data/neformatiraneOsobe.txt");
        assertEquals(0, om.dajSveOsobe().size());
    }

    @Test
    void ispravanBrojOsoba() {
        OsobaModel om = new OsobaModel();
        try {
            om.napuniPodatkeIzTxtDatoteke("src/data/osobe.txt");
            assertEquals(2, om.dajSveOsobe().size());
        } catch (Exception e) {
            fail("Exception thrown: " + e.getMessage());
        }
    }

    @Test
    void ispravnoImeOsobe() {
        OsobaModel om = new OsobaModel();
        try {
            om.napuniPodatkeIzTxtDatoteke("src/data/osobe.txt");
            assertEquals("John", om.dajOsobuPoId(1).getIme());
            assertEquals("Alice", om.dajOsobuPoId(2).getIme());

            assertEquals("Doe", om.dajOsobuPoId(1).getPrezime());
            assertEquals("Alister", om.dajOsobuPoId(2).getPrezime());

            assertEquals("1501995123456", om.dajOsobuPoId(1).getMaticniBroj());
            assertEquals("2005005444444", om.dajOsobuPoId(2).getMaticniBroj());

            assertEquals("Some Address", om.dajOsobuPoId(1).getAdresa());
            assertEquals("Another Address", om.dajOsobuPoId(2).getAdresa());

            assertEquals(dateFormat.parse("1995-01-15").toString(), om.dajOsobuPoId(1).getDatumRodjenja().toString());
            assertEquals(dateFormat.parse("2005-05-20").toString(), om.dajOsobuPoId(2).getDatumRodjenja().toString());

            assertEquals(Uloga.STUDENT, om.dajOsobuPoId(1).getUloga());
            assertEquals(Uloga.NASTAVNO_OSOBLJE, om.dajOsobuPoId(2).getUloga());
        } catch (Exception e) {
            fail("Exception thrown: " + e.getMessage());
        }
    }

    @Test
    void azuriranjeNepostojeceg() throws IOException, ParseException {
        OsobaModel om = new OsobaModel();
        om.napuniPodatkeIzTxtDatoteke("src/data/osobe.txt");
        String ret = om.azurirajOsobu(3, "Kirba", "Bax", "Adresa", new Date(96, 0, 1), "0101996444444", Uloga.NASTAVNO_OSOBLJE);
        assertEquals("Osoba nije pronadjena!", ret);
    }
    @Test
    void azuriranjePostojeceg() throws IOException, ParseException {
        OsobaModel om = new OsobaModel();
        om.napuniPodatkeIzTxtDatoteke("src/data/osobe.txt");
        String ret = om.azurirajOsobu(2, "Kirba", "Bax", "Adresa", new Date(96, 0, 1), "0101996444444", Uloga.NASTAVNO_OSOBLJE);
        assertEquals("Osoba je uspjesno azurirana!", ret);
    }
    @Test
    void azuriranjeParcijalno() {
        OsobaModel om = new OsobaModel();
        try {
            om.napuniPodatkeIzTxtDatoteke("src/data/osobe.txt");
            om.azurirajOsobu(2, "Kirba", null, null, null, null, null);
            assertEquals("Kirba", om.dajOsobuPoId(2).getIme());
            assertEquals("Alister", om.dajOsobuPoId(2).getPrezime());
            assertEquals("Another Address", om.dajOsobuPoId(2).getAdresa());
            assertEquals(dateFormat.parse("2005-05-20").toString(), om.dajOsobuPoId(2).getDatumRodjenja().toString());
            assertEquals("2005005444444", om.dajOsobuPoId(2).getMaticniBroj());
            assertEquals(Uloga.NASTAVNO_OSOBLJE, om.dajOsobuPoId(2).getUloga());
        } catch (Exception e) {
            fail("Exception thrown: " + e.getMessage());
        }
    }
}