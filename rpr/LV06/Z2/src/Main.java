import controller.PredmetController;
import model.Predmet;
import view.PredmetView;

import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Predmet p = new Predmet("Razvoj softvera", 6.0);
        PredmetView pv = new PredmetView();
        PredmetController pc = new PredmetController(p, pv);
        while(true) {
            System.out.println("Odaberite opciju:");
            System.out.println("(1) Prikazi predmet");
            System.out.println("(2) Promijeni predmet");
            System.out.println("(3) Prikazi ECTS");
            System.out.println("(4) Promijeni ECTS");
            System.out.println("Bilo sta drugo zavrsava program");
            Scanner s = new Scanner(System.in);
            int opcija = s.nextInt();
            boolean kraj = false;
            switch (opcija) {
                case 1:
                    pc.dajPredmet();
                    System.out.println(pv.getPoruka());
                    break;
                case 2:
                    System.out.println("Unesite novi naziv predmeta:");
                    s.nextLine();
                    String naziv = s.nextLine();
                    pc.azurirajPredmet(naziv);
                    System.out.println(pv.getPoruka());
                    break;
                case 3:
                    pc.dajECTS();
                    System.out.println(pv.getPoruka());
                    break;
                case 4:
                    System.out.println("Unesite novi ECTS:");
                    Double ECTS = s.nextDouble();
                    pc.azurirajECTS(ECTS);
                    System.out.println(pv.getPoruka());
                    break;
                default:
                    System.out.println("Nepostojeca opcija");
                    kraj = true;
            }
            if(kraj) break;
        }
    }
}