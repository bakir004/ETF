import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

public class SortiranjeNit extends Thread {
    private void bubbleSort(List<AtomicInteger> list) {
        int n = list.size();
        int temp = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 1; j < (n - i); j++) {
                if (list.get(j - 1).get() > list.get(j).get()) {
                    synchronized (list) {
                        temp = list.get(j - 1).get();
                        list.set(j - 1, list.get(j));
                        list.set(j, new AtomicInteger(temp));
                    }
                }
            }
        }
    }
    public void run() {
        bubbleSort(Main.lista);
    }
}
