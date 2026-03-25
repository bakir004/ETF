package com.example.z2.model;

import javafx.beans.property.*;

public class Predmet {
    IntegerProperty id;
    StringProperty naziv;
    DoubleProperty ECTS;
    public Predmet(Integer id, String naziv, Double ECTS) {
        this.id = new SimpleIntegerProperty();
        this.naziv = new SimpleStringProperty();
        this.ECTS = new SimpleDoubleProperty();
        setId(id);
        setNaziv(naziv);
        setECTS(ECTS);
    }

    public int getId() {
        return id.get();
    }

    public IntegerProperty idProperty() {
        return id;
    }

    public void setId(int id) {
        if(id < 0) throw new IllegalArgumentException("ID mora biti pozitivan broj");
        this.id.set(id);
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

    public String toString() {
        return "("+ id.get() +
                ") " + naziv.get() +
                ", ECTS: " + ECTS.get();
    }
}
