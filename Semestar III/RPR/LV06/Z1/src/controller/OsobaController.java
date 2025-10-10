package controller;

import model.Osoba;
import view.OsobaView;

import java.util.List;

public class OsobaController {
    private Osoba model;
    private OsobaView view;

    public OsobaController(Osoba model, OsobaView view)
    {
        this.model = model;
        this.view = view;
    }

    public Osoba getModel() {
        return model;
    }

    public void setModel(Osoba model) {
        this.model = model;
    }

    public OsobaView getView() {
        return view;
    }

    public void setView(OsobaView view) {
        this.view = view;
    }

    public void azurirajIme()
    {
        try {
            model.setIme(view.getUlazniTekst());
            view.setPoruka("Ime je uspjesno azurirano!");
        }
        catch(Exception e) {
            view.setPoruka("Greska: " + e.getMessage());
        }
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

}
