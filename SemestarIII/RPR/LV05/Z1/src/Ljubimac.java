import java.util.Date;

public abstract class Ljubimac implements Objekat {
    private String ime;
    private Date datumRodjenja;
    private String bolesti;

    public Ljubimac(String ime, Date datumRodjenja, String bolesti) {
        this.ime = ime;
        this.datumRodjenja = datumRodjenja;
        this.bolesti = bolesti;
    }

    public String getIme() {
        return ime;
    }

    public void setIme(String ime) {
        this.ime = ime;
    }

    public Date getDatumRodjenja() {
        return datumRodjenja;
    }

    public void setDatumRodjenja(Date datumRodjenja) {
        this.datumRodjenja = datumRodjenja;
    }

    public String getBolesti() {
        return bolesti;
    }

    public void setBolesti(String bolesti) {
        this.bolesti = bolesti;
    }
}
