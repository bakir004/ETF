package com.example.z1.view;

public class OsobaView {
    private String ulazniTekst;
    private String poruka;

    public OsobaView() {
        this.ulazniTekst = "";
        this.poruka = "";
    }

    public String getUlazniTekst() {
        return ulazniTekst;
    }

    public void setUlazniTekst(String ulazniTekst) {
        this.ulazniTekst = ulazniTekst;
    }

    public String getPoruka() {
        return poruka;
    }

    public void setPoruka(String poruka) {
        this.poruka = poruka;
    }
}