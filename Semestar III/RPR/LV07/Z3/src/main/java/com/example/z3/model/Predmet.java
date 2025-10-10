package com.example.z3.model;

import javafx.beans.property.DoubleProperty;
import javafx.beans.property.SimpleDoubleProperty;
import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;

public class Predmet {
    StringProperty naziv;
    DoubleProperty ECTS;
    public Predmet(String naziv, Double ECTS) {
        this.naziv = new SimpleStringProperty();
        this.ECTS = new SimpleDoubleProperty();
        setNaziv(naziv);
        setECTS(ECTS);
    }

    public String getNaziv() {
        return naziv.get();
    }

    public StringProperty nazivProperty() {
        return naziv;
    }

    public void setNaziv(String naziv) {
        if (naziv.length() < 5 || naziv.length() > 100) {
            throw new IllegalArgumentException("Naziv mora imati dužinu između 5 i 100 znakova");
        }
        this.naziv.set(naziv);
    }

    public Double getECTS() {
        return ECTS.get();
    }

    public DoubleProperty ECTSProperty() {
        return ECTS;
    }

    public void setECTS(Double ECTS) {
        if (ECTS < 5.0 || ECTS > 20.0) {
            throw new IllegalArgumentException("ECTS mora biti izmedju 5.0 i 20.0");
        }
        if (ECTS*10 % 5 != 0) {
            throw new IllegalArgumentException("ECTS mora biti cijeli broj ili broj sa jednom decimalom");
        }
        this.ECTS.set(ECTS);
    }
}
