import Interfaces.IZdravlja;

public class Prodavac implements IZdravlja {
    private String ime;
    private String prezime;
    private String brojStanda;
    private Integer ID;

    public Prodavac(String ime, String prezime, String brojStanda, Integer ID) {
        this.ime = ime;
        this.prezime = prezime;
        this.brojStanda = brojStanda;
        this.ID = ID;
    }

    public String getIme() {
        return ime;
    }

    public void setIme(String ime) {
        this.ime = ime;
    }

    public String getPrezime() {
        return prezime;
    }

    public void setPrezime(String prezime) {
        this.prezime = prezime;
    }

    public String getBrojStanda() {
        return brojStanda;
    }

    public void setBrojStanda(String brojStanda) {
        this.brojStanda = brojStanda;
    }

    public Integer getID() {
        return ID;
    }

    public void setID(Integer ID) {
        this.ID = ID;
    }

    @Override
    public Boolean Zdravlje() {
        return ID % 100 == 1;
    }
}
