import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    public static List<AtomicInteger> lista;
    public static void main(String[] args) {
        int N = 10000;
        int numThreads = 4;
        lista = new ArrayList<>();
        for(int i = N; i >= 0; i--) {
            lista.add(new AtomicInteger(i));
        }
        List<SortiranjeNit> niti = new ArrayList<>();
        for(int i = 0; i < numThreads; i++) {
            niti.add(new SortiranjeNit());
            niti.get(i).start();
        }
        try {
            for(SortiranjeNit nit : niti) {
                nit.join();
            }
            System.out.println("Sortirana lista!\n" + lista);
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }
}