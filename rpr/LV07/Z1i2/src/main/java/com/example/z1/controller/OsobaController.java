package com.example.z1.controller;

import com.example.z1.model.Osoba;
import com.example.z1.model.OsobaModel;
import com.example.z1.view.OsobaView;

import java.util.List;

public class OsobaController
{
    private OsobaModel model;
    private OsobaView view;


    public OsobaController(OsobaModel model, OsobaView view)
    {
        this.model = model;
        this.view = view;
    }

    public OsobaModel getModel() {
        return model;
    }

    public void setModel(OsobaModel model) {
        this.model = model;
    }

    public OsobaView getView() {
        return view;
    }

    public void setView(OsobaView view) {
        this.view = view;
    }

    public void dajOsobeIzTxtDatoteke(String filePath)
    {
        try
        {
            List<Osoba> osobe = Osoba.ucitajOsobeIzTxtDatoteke(filePath);
            String poruka = "Osobe ucitane iz txt datoteke su:\n";
            for (Osoba osoba : osobe)
            {
                poruka += osoba.toString() + "\n";
            }
            view.setPoruka(poruka);
        }
        catch(Exception e)
        {
            view.setPoruka("Greska: " + e.getMessage());
        }
    }

    public void dajOsobeIzXmlDatoteke(String filePath)
    {
        try
        {
            List<Osoba> osobe = Osoba.ucitajOsobeIzXmlDatoteke(filePath);
            String poruka = "Osobe ucitane iz txt datoteke su:\n";
            for (Osoba osoba : osobe)
            {
                poruka += osoba.toString() + "\n";
            }
            view.setPoruka(poruka);
        }
        catch(Exception e)
        {
            view.setPoruka("Greska: " + e.getMessage());
        }
    }


    public Osoba dajOsobuPoId(Integer id){
        Osoba pronadjenaOsoba = model.dajOsobuPoId(id);
        if(pronadjenaOsoba == null)
            view.setPoruka("Osoba nije pronadjena!");
        else
            view.setPoruka("Pronadjena osoba: " + pronadjenaOsoba.toString());
        return pronadjenaOsoba;
    }


    public void azurirajIme(Integer id){
        try {
            model.azurirajOsobu(id, view.getUlazniTekst(), null, null, null, null, null);
            view.setPoruka("Ime je uspjesno azurirano!");
        }
        catch(Exception e){
            view.setPoruka("Greska: " + e.getMessage());
        }
    }
}
