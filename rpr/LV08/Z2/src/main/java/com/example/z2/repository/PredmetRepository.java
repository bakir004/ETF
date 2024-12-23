package com.example.z2.repository;

import com.example.z2.model.Predmet;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

public class PredmetRepository {
    private Integer nextId = 1;
    private ObservableList<Predmet> predmeti;

    public PredmetRepository() {
        predmeti = FXCollections.observableArrayList();
    }

    public void seed() {
        Predmet p1 = new Predmet(nextId, "Razvoj programskih problema", 6.0);
        nextId++;
        Predmet p2 = new Predmet(nextId, "Uvod u kulturu zivljenja", 7.0);
        nextId++;
        Predmet p3 = new Predmet(nextId, "Osnove pirotehnike", 9.0);
        nextId++;
        predmeti.addAll(p1, p2, p3);
    }

    public ObservableList<Predmet> dajSvePredmete() {
        return predmeti;
    }

    public Predmet dajPredmetPoId(Integer id) {
        for (Predmet p : predmeti) {
            if (p.getId() == id) {
                return p;
            }
        }
        return null;
    }

    public void dodajPredmet(String naziv, Double ECTS) {
        Predmet predmet = new Predmet(nextId, naziv, ECTS);
        nextId++;
        predmeti.add(predmet);
    }

    public void azurirajPredmet(Predmet predmet) {
        for (Predmet p : predmeti) {
            if (p.getId() == predmet.getId()) {
                p.setNaziv(predmet.getNaziv());
                p.setECTS(predmet.getECTS());
                return;
            }
        }
        throw new IllegalArgumentException("Predmet s ID-em " + predmet.getId() + " ne postoji");
    }

    public void obrisiPredmetPoId(Integer id) {
        for (Predmet p : predmeti) {
            if (p.getId() == id) {
                predmeti.remove(p);
                return;
            }
        }
        throw new IllegalArgumentException("Predmet s ID-em " + id + " ne postoji");
    }

    public void obrisiSvePredmete() {
        predmeti.clear();
    }
}
