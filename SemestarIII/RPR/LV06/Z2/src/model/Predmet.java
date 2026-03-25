package model;

public class Predmet {
    String naziv;
    Double ECTS;
    public Predmet(String naziv, Double ECTS) {
        setNaziv(naziv);
        setECTS(ECTS);
    }

    public String getNaziv() {
        return naziv;
    }

    public void setNaziv(String naziv) {
        if (naziv.length() < 5 || naziv.length() > 100) {
            throw new IllegalArgumentException("Naziv mora imati dužinu između 5 i 100 znakova");
        }
        this.naziv = naziv;
    }

    public Double getECTS() {
        return ECTS;
    }

    public void setECTS(Double ECTS) {
        if (ECTS < 5.0 || ECTS > 20.0) {
            throw new IllegalArgumentException("ECTS mora biti izmedju 5.0 i 20.0");
        }
        if (ECTS*10 % 5 != 0) {
            throw new IllegalArgumentException("ECTS mora biti cijeli broj ili broj sa jednom decimalom");
        }
        this.ECTS = ECTS;
    }
}
