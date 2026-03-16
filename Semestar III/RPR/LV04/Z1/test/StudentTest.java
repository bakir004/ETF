import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import java.util.Date;

import static org.junit.jupiter.api.Assertions.*;

class StudentTest {
    private static Student s;
    @BeforeAll
    static void beforeAll() {
        s = new Student("Student", "1", "", new Date(98, 2, 2), "12345", 2, 0.0);
    }

    @Test
    void dajInformacije() {
        assertEquals("Student: Student 1, broj indeksa: 12345", s.DajInformacije());
    }
}