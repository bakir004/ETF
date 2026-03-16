import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Main {
    public static List<Integer> nums;
    public static int trazeniBroj;
    public static boolean pronadjen = false;
    public static void main(String[] args) {
        List<PretragaNit> niti = new ArrayList<>();
        nums = new ArrayList<>();
        int N = 1000000;
        int numThreads = 16;
        Random random = new Random();
        for(int i = 0; i < N; i++) {
            int broj = random.nextInt(0, N);
            nums.add(broj);
        }
        System.out.println("Ocekujemo na poziciji 2...");
        trazeniBroj = nums.get(2);
        for(int i = 0; i < numThreads; i++) {
            PretragaNit nit = new PretragaNit(i * (N / numThreads), (i + 1) * (N / numThreads));
            niti.add(nit);
            nit.start();
        }
        try {
            for(PretragaNit nit : niti) {
                nit.join();
            }
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }
}