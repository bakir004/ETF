import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Main {
    public static double Plus(double broj1, double broj2) {
        return broj1+broj2;
    }
    public static double Podijeljeno(double broj1, double broj2) throws Exception {
        if(broj2 == 0) throw new Exception("DIJELJENJE S NULOM");
        return broj1/broj2;
    }
    public static void main(String[] args) {
        System.out.println("Unesite operaciju ('plus' za sabiranje, 'podijeljeno' za dijeljenje):");
        Scanner in = new Scanner(System.in);
        String opcija = in.nextLine();
        List<Double> brojevi = new ArrayList<Double>();
        while(true) {
            double unos = in.nextDouble();
            if(unos == -400) break;
            brojevi.add(unos);
        }
        if(brojevi.isEmpty()) return;
        double rezultat = brojevi.get(0);
        try {
            if(opcija.equals("plus")) {
                for(int i = 1; i < brojevi.size(); i++)
                    rezultat = Plus(rezultat, brojevi.get(i));
            } else if(opcija.equals("podijeljeno")) {
                for(int i = 1; i < brojevi.size(); i++)
                    rezultat = Podijeljeno(rezultat, brojevi.get(i));
            }
        } catch(Exception e) {
            System.out.println("GRESKA: " + e.getMessage());
        }
        System.out.println("Rezultat: " + (Math.round(rezultat * 100)) / 100.0);
    }
}