import java.util.Scanner;
import java.util.*;

public class Main {
    public static void main(String args[]) {
        Scanner s = new Scanner(System.in);
        List<Integer> l = new ArrayList();
        for(int i = 0; i < 10; i++) {
            Integer n = s.nextInt();
            l.add(n);
        }
        l.sort(Comparator.comparingInt(n -> n));
        System.out.print("[");
        List<Integer> rest = new ArrayList();
        for(int i = 0; i < 10; i++) {
            if((i == 0 && l.get(i) % 2 == 0) || (l.get(i) % 2 == 0 && i>0 && l.get(i-1)!=l.get(i))) {
                rest.add(l.get(i));
            }
        }
        for(int i = 0; i < rest.size(); i++) {
            System.out.print(rest.get(i));
            if(i != rest.size()-1) {
                System.out.print(", ");
            }
        }
        System.out.print("]");
    }
}