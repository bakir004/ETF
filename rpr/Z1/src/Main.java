import java.util.ArrayList;
import java.util.Scanner;

public class Main {
    public static void printPrimes(int n) {
        ArrayList<Boolean> primes = new ArrayList<Boolean>(n+1);
        for (int i = 0; i <= n; i++) { primes.add(true); }
        primes.set(0, false);
        // OVAJ PUT IMA KOMENTAR
        primes.set(1, false);
        for(int i = 2; i*i <= n; i++) {
            if(!primes.get(i)) continue;
            for(int j = i * i; j <= n; j+=i)
                primes.set(j, false);
        }
        for(int i = 2; i <= n; i++)
            if(primes.get(i))
                System.out.printf(i + " ");
    }
    public static void main(String[] args) {
        int n = -1;
        while(true) {
            System.out.println("Unesite broj n:");
            Scanner in = new Scanner(System.in);
            n = in.nextInt();
            if(n <= 2) {
                System.out.println("Nije moguce izvrsiti izracunavanje prostih brojeva");
                return;
            }
            if(n <= 500) break;
            System.out.println("Broj je prevelik");
        }
        printPrimes(2*n);
    }
}