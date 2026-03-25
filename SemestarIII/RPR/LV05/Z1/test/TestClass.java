import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

public class TestClass
{
    static List<Ljubimac> ljubimci = new ArrayList<Ljubimac>();
    static Veterinar veterinar;


    @BeforeAll
    public static void Inicijalizacija ()
    {
        ljubimci.add(new Pas("Pas", new Date(118, 4, 20), "Nema bolesti", VrstaPsa.ZlatniRetreiver));
        ljubimci.add(new Macka("Macka", new Date(120, 10, 1), "Nema bolesti", VrstaMacke.Sijamska));
        veterinar = new Veterinar("Doktor", Specijalizacija.Psi);
    }


    @Test
    public void TestVeterinarskihUsluga ()
    {
        veterinar.PregledajLjubimca(ljubimci.get(0));
        assertTrue(veterinar.getPregledi().size() == 1);
        assertThrows(ValidacijaVrsteException.class, () ->
        {
            veterinar.PregledajLjubimca(ljubimci.get(1));
        });
    }

    @Test
    public void TestPrikazaInformacija ()
    {
        String rezultat = "";
        List<Objekat> objekti = new ArrayList<Objekat>();
        objekti.add(ljubimci.get(0));
        objekti.add(ljubimci.get(1));
        objekti.add(veterinar);


        for (Objekat o : objekti)
        {
            rezultat += o.PrikaziInformacije() + " ";
        }
        assertEquals(rezultat, "Pas: ZlatniRetreiver Mačka: Sijamska Veterinar: Doktor ");
    }
}

