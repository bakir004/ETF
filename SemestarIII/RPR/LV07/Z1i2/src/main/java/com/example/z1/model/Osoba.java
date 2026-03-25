package com.example.z1.model;

import javafx.beans.property.*;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class Osoba {
    private IntegerProperty id;
    private StringProperty ime, prezime, adresa;
    private ObjectProperty<Date> datumRodjenja;
    private StringProperty maticniBroj;
    private ObjectProperty<Uloga> uloga;

    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");

    public Osoba(Integer id, String ime, String prezime, String adresa, Date datumRodjenja, String maticniBroj, Uloga uloga) {
        this.id = new SimpleIntegerProperty(id);
        this.ime = new SimpleStringProperty();
        this.prezime = new SimpleStringProperty(prezime);
        this.adresa = new SimpleStringProperty(adresa);
        this.datumRodjenja = new SimpleObjectProperty<>(datumRodjenja);
        this.maticniBroj = new SimpleStringProperty();
        this.uloga = new SimpleObjectProperty<>(uloga);


        setIme(ime);
        setMaticniBroj(maticniBroj);
    }

    public int getId() {
        return id.get();
    }

    public IntegerProperty idProperty() {
        return id;
    }

    public void setId(int id) {
        this.id.set(id);
    }

    public String getIme() {
        return ime.get();
    }

    public StringProperty imeProperty() {
        return ime;
    }

    public String getPrezime() {
        return prezime.get();
    }

    public StringProperty prezimeProperty() {
        return prezime;
    }

    public void setPrezime(String prezime) {
        this.prezime.set(prezime);
    }

    public String getAdresa() {
        return adresa.get();
    }

    public StringProperty adresaProperty() {
        return adresa;
    }

    public void setAdresa(String adresa) {
        this.adresa.set(adresa);
    }

    public Date getDatumRodjenja() {
        return datumRodjenja.get();
    }

    public ObjectProperty<Date> datumRodjenjaProperty() {
        return datumRodjenja;
    }

    public void setDatumRodjenja(Date datumRodjenja) {
        this.datumRodjenja.set(datumRodjenja);
    }

    public String getMaticniBroj() {
        return maticniBroj.get();
    }

    public StringProperty maticniBrojProperty() {
        return maticniBroj;
    }

    public Uloga getUloga() {
        return uloga.get();
    }

    public ObjectProperty<Uloga> ulogaProperty() {
        return uloga;
    }

    public void setUloga(Uloga uloga) {
        this.uloga.set(uloga);
    }

    public void setIme(String ime)
    {
        if (ime == null || ime.length() < 2 || ime.length() > 50) {
            throw new IllegalArgumentException("Ime mora imati izmedju 2 i 50 znakova.");
        }
        this.ime.set(ime);
    }

    public void setMaticniBroj(String maticniBroj) {
        if (maticniBroj == null || maticniBroj.trim().isEmpty() || maticniBroj.length() != 13) {
            throw new IllegalArgumentException("Maticni broj mora imati tacno 13 karaktera");
        }
        else if(!ProvjeriMaticniBroj(maticniBroj)){
            throw new IllegalArgumentException("Maticni broj se ne poklapa sa datumom rodjenja!");
        }
        this.maticniBroj.set(maticniBroj);
    }

    public boolean ProvjeriMaticniBroj(String maticniBroj)
    {
        boolean danIsti = getDatumRodjenja().getDate() == Integer.parseInt(maticniBroj.substring(0, 2)),
                mjesecIsti = getDatumRodjenja().getMonth()+1 == Integer.parseInt(maticniBroj.substring(2, 4)),
                godinaIsta = (getDatumRodjenja().getYear()+900 > 1000 ? getDatumRodjenja().getYear()+900-1000 : getDatumRodjenja().getYear()+900)  == Integer.parseInt(maticniBroj.substring(4, 7));
        return (danIsti && mjesecIsti && godinaIsta);
    }
    public boolean mozeUcestvovatiUProjektu(boolean voditeljProjekta)
    {
        if(this.uloga.get() == Uloga.NASTAVNO_OSOBLJE || (!voditeljProjekta && this.uloga.get() == Uloga.STUDENT))
        {
            return true;
        }
        return false;
    }

    public static List<Osoba> ucitajOsobeIzTxtDatoteke(String putanjaDoDatoteke) throws IOException, ParseException {
        List<Osoba> osobe = new ArrayList<>();
        BufferedReader reader = new BufferedReader(new FileReader(putanjaDoDatoteke));
        String linija;
        while ((linija = reader.readLine()) != null) {
            String[] polja = linija.split(",");
            if (polja.length == 7) {
                Integer id = Integer.parseInt(polja[0]);
                String ime = polja[1];
                String prezime = polja[2];
                String adresa = polja[3];
                Date datumRodjenja = dateFormat.parse(polja[4]);
                String maticniBroj = polja[5];
                Uloga uloga = Uloga.valueOf(polja[6].toUpperCase());

                Osoba osoba = new Osoba(id, ime, prezime, adresa, datumRodjenja, maticniBroj,uloga);
                osobe.add(osoba);
            }
        }
        reader.close();
        return osobe;
    }
    public static List<Osoba> ucitajOsobeIzXmlDatoteke(String putanjaDoDatoteke) throws Exception {
        List<Osoba> osobe = new ArrayList<>();
        File xmlDatoteka = new File(putanjaDoDatoteke);

        DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
        Document doc = builder.parse(xmlDatoteka);

        doc.getDocumentElement().normalize();
        NodeList listaCvorova = doc.getElementsByTagName("osoba");

        for (int i = 0; i < listaCvorova.getLength(); i++) {
            Node cvor = listaCvorova.item(i);


            if (cvor.getNodeType() == Node.ELEMENT_NODE) {
                Element element = (Element) cvor;

                Integer id = Integer.parseInt(element.getElementsByTagName("id").item(0).getTextContent());
                String ime = element.getElementsByTagName("ime").item(0).getTextContent();
                String prezime = element.getElementsByTagName("prezime").item(0).getTextContent();
                String adresa = element.getElementsByTagName("adresa").item(0).getTextContent();
                Date datumRodjenja = dateFormat.parse(element.getElementsByTagName("datumRodjenja").item(0).getTextContent());
                String maticniBroj = element.getElementsByTagName("maticniBroj").item(0).getTextContent();
                Uloga uloga = Uloga.valueOf(element.getElementsByTagName("uloga").item(0).getTextContent().toUpperCase());

                Osoba osoba = new Osoba(id, ime, prezime, adresa, datumRodjenja, maticniBroj, uloga);
                osobe.add(osoba);
            }
        }
        return osobe;
    }
    public boolean imaPravoNaStipendiju() {
        return this.uloga.get() == Uloga.STUDENT;
    }

    @Override
    public String toString() {
        return "Osoba{" +
                "id='" + id + '\'' +
                ", ime='" + ime + '\'' +
                ", prezime='" + prezime + '\'' +
                ", adresa='" + adresa + '\'' +
                ", datumRodjenja=" + datumRodjenja +
                ", maticniBroj='" + maticniBroj + '\'' +
                ", uloga=" + uloga +
                '}';
    }
}
