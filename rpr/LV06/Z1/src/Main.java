import controller.OsobaController;
import model.Osoba;
import model.Uloga;
import view.OsobaView;

import java.util.Date;

public class Main {
    public static void main(String[] args) {
        OsobaView osobaView = new OsobaView();
        Osoba osoba = new Osoba(1, "Bakir", "Cinjarevic", "Inat kuca", new Date(95, 0, 1), "0101995170001", Uloga.STUDENT);
        OsobaController osobaController = new OsobaController(osoba, osobaView);
        osobaController.dajOsobeIzTxtDatoteke("src/data/osobe.txt");
        System.out.println("2) View ispisuje: " + osobaView.getPoruka());
    }
}