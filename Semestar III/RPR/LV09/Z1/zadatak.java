public Osoba dajOsobuPoId(Integer id) {
    String upit = "SELECT * FROM osoba WHERE id = ?";
    try (Connection konekcija = DriverManager.getConnection(url);
         PreparedStatement ps = konekcija.prepareStatement(upit)) {
        ps.setInt(1, id);
        ResultSet rs = ps.executeQuery();
        if (rs.next()) {
            return new Osoba(
                    rs.getInt("id"),
                    rs.getString("ime"),
                    rs.getString("prezime"),
                    rs.getString("adresa"),
                    rs.getDate("datumRodjenja"),
                    rs.getString("maticniBroj"),
                    Uloga.valueOf(rs.getString("uloga"))
            );
        }
    } catch (SQLException e) {
        return null;
    }
    return null;
}
public String obrisiOsobuPoId(Integer id) {
    Osoba osoba = dajOsobuPoId(id);
    if(osoba == null) {
        return "Ne postoji osoba sa datim id-em";
    }
    String upit = "DELETE FROM osoba WHERE id = ?";
    try (Connection konekcija = DriverManager.getConnection(url);
         PreparedStatement ps = konekcija.prepareStatement(upit)) {
        ps.setInt(1, id);
        int brojObrisanihRedova = ps.executeUpdate();
        return "Osoba je uspjesno obrisana";
    } catch (SQLException e) {
        return e.getMessage();
    }
}
public boolean provjeriMaticniBroj(String maticniBroj, Date datumRodjenja) {
    boolean danIsti = datumRodjenja.getDate() == Integer.parseInt(maticniBroj.substring(0, 2)),
    mjesecIsti = datumRodjenja.getMonth()+1 == Integer.parseInt(maticniBroj.substring(2, 4)),
    godinaIsta = (datumRodjenja.getYear()+900 > 1000 ? datumRodjenja.getYear()+900-1000 : datumRodjenja.getYear()+900)  == Integer.parseInt(maticniBroj.substring(4, 7));
    return (danIsti && mjesecIsti && godinaIsta);
}
public String azurirajOsobu(Integer id, String novoIme, String novoPrezime, String novaAdresa, Date noviDatumRodjenja, String noviMaticniBroj, Uloga novaUloga) {
    if(!provjeriMaticniBroj(noviMaticniBroj, noviDatumRodjenja)) {
        noviMaticniBroj = null;
    }
    if (novoIme.length() < 2 || novoIme.length() > 50) {
        novoIme = null;
    }
    Osoba trazenaOsoba = dajOsobuPoId(id);
    if(trazenaOsoba != null) {
        try {
            if (novoIme != null) {
                trazenaOsoba.setIme(novoIme);
            }
            if (novoPrezime != null) {
                trazenaOsoba.setPrezime(novoPrezime);
            }
            if (novaAdresa != null) {
                trazenaOsoba.setAdresa(novaAdresa);
            }
            if (noviDatumRodjenja != null) {
                trazenaOsoba.setDatumRodjenja(noviDatumRodjenja);
            }
            if (noviMaticniBroj != null) {
                trazenaOsoba.setMaticniBroj(noviMaticniBroj);
            }
            if (novaUloga != null){
                trazenaOsoba.setUloga(novaUloga);
            }
            return "Osoba je uspjesno azurirana!";
        }
        catch (IllegalArgumentException e) {
            return e.getMessage();
        }
    }
    return "Osoba nije pronadjena!";
}
