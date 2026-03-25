public class PretragaNit extends Thread{
    Integer pocetak, kraj;
    public PretragaNit(Integer pocetak, Integer kraj)
    {
        this.pocetak = pocetak;
        this.kraj = kraj;
    }
    public void run ()
    {
        for (int i = pocetak; i < kraj; i++) {
            if(Main.pronadjen) {
                return;
            }
            if(Main.nums.get(i) == Main.trazeniBroj) {
                Main.pronadjen = true;
                System.out.println("Pronadjen broj na poziciji: " + i);
                System.out.println("Element: " + Main.nums.get(i));
            }
        }
    }
}
