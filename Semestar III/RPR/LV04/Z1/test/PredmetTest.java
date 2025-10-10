import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import java.util.Date;

import static org.junit.jupiter.api.Assertions.*;

class PredmetTest {
    private static Predmet p;

    @BeforeAll
    static void beforeAll() {
        p = new Predmet("RPR", new Profesor("Profesor", "1", "", new Date(99, 0, 1), 150, 2000, "3-00", "red. prof. dr.", 50), null, 6.0);
    }

    @Test
    void dajInformacije() {
        assertEquals("Predmet: RPR, odgovorni profesor: Profesor: red. prof. dr. Profesor 1", p.DajInformacije());
    }
}