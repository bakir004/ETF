package com.example.z3.controller;


import com.example.z3.model.Predmet;
import com.example.z3.view.PredmetView;

public class PredmetController {
    private Predmet model;
    private PredmetView view;

    public PredmetController(Predmet model, PredmetView view) {
        this.model = model;
        this.view = view;
    }

    public void dajPredmet() {
        view.setPoruka(model.getNaziv());
    }
    public void dajECTS() {
        view.setPoruka(model.getECTS().toString());
    }
    public void azurirajPredmet(String naziv) {
        try {
            model.setNaziv(naziv);
            view.setPoruka("Azuriran naziv predmeta na " + model.getNaziv());
        } catch (IllegalArgumentException e) {
            view.setPoruka(e.getMessage());
        }
    }
    public void azurirajECTS(Double ECTS) {
        try {
            model.setECTS(ECTS);
            view.setPoruka("Azuriran ECTS predmeta na " + model.getECTS());
        } catch (IllegalArgumentException e) {
            view.setPoruka(e.getMessage());
        }
    }
}
