import Izuzeci.PremladStudentException;
import Izuzeci.StudentBuducnostException;
import Klase.Odsjek;
import Klase.Student;

import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        boolean uspjesanUnos = false;
        Student student = null;
        while (!uspjesanUnos) {
            try {
                System.out.printf("Datum rođenja (dd/mm/yyyy):");
                String datumString = scanner.nextLine();
                int godina = Integer.valueOf(datumString.substring(6)) - 1900;
                int mjesec = Integer.valueOf(datumString.substring(3, 5)) - 1;
                int dan = Integer.valueOf(datumString.substring(0, 2));
                Date datumDate = new Date(godina, mjesec, dan);
                student = new Student("Ime", "Prezime", datumDate, "12345", Odsjek.RI, 2);
                uspjesanUnos = true;
                System.out.println("Unesite ocjene studenta:");
                String nizOcjena = scanner.nextLine();
                String[] stringOcjene = nizOcjena.split(",");
                List<Integer> ocjene = new ArrayList<Integer>();
                Arrays.stream(stringOcjene).forEach(strOcjena -> ocjene.add(Integer.parseInt(strOcjena)));
                student.setOcjene(ocjene);
            } catch (PremladStudentException e) {
                System.out.println(e.getMessage());
                return;
            } catch (StudentBuducnostException e) {
                System.out.println(e.getMessage());
                System.out.println("Molimo ponovite unos datuma rođenja!");
            } catch (Exception e) {
                throw new RuntimeException(e);
            }
        }
        System.out.println("Unos studenta uspješan! " + student);
    }
}