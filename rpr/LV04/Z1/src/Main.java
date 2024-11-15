import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        Profesor p = new Profesor("Profesor", "1", "", new Date(99, 0, 1), 150, 2000, "3-00", "red. prof. dr.", 50);
        Student s = new Student("Student", "1", "", new Date(98, 2, 2), "12345", 2, 0.0);
        Predmet pr = new Predmet("RPR", p, null, 5.0);

        List<IInformacije> objekti = new ArrayList<IInformacije>();
        objekti.add(p);
        objekti.add(s);
        objekti.add(pr);
        for (IInformacije o : objekti)
            System.out.println(o.DajInformacije());
    }
}