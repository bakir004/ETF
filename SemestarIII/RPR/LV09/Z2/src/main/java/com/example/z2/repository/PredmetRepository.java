package com.example.z2.repository;

import com.example.z2.model.Predmet;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

import java.sql.*;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.List;

public class PredmetRepository {
    private Integer nextId = 1;

    private static final String DATABASE_URL = "jdbc:sqlite:baza.db";
    private static Connection connect() throws SQLException {
        return DriverManager.getConnection(DATABASE_URL);
    }

    public void kreirajTabeluPredmetaAkoNePostoji() {
        String createTableSql = """
                CREATE TABLE IF NOT EXISTS predmeti (
                    id INTEGER PRIMARY KEY,
                    naziv TEXT NOT NULL,
                    ECTS REAL NOT NULL
                );
                """;
        try (Connection conn = connect();
             Statement stmt = conn.createStatement()) {
            stmt.execute(createTableSql);
            System.out.println("Tabela je kreirana ili vec postoji!");
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
    }

    public void popuniPredmete() {
        String insertSQL = "INSERT INTO predmeti (id, naziv, ECTS) VALUES (?, ?, ?);";

        try (Connection conn = connect();
             PreparedStatement pstmt = conn.prepareStatement(insertSQL)) {

            pstmt.setInt(1, 1);
            pstmt.setString(2, "Razvoj programskih problema");
            pstmt.setDouble(3, 6.0);
            pstmt.executeUpdate();
            pstmt.setInt(1, 2);
            pstmt.setString(2, "Uvod u kulturu zivljenja");
            pstmt.setDouble(3, 7.0);
            pstmt.executeUpdate();
            pstmt.setInt(1, 3);
            pstmt.setString(2, "Osnove pirotehnike");
            pstmt.setDouble(3, 9.0);
            pstmt.executeUpdate();

            System.out.println("Ubaceni pocetni podaci!");
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
        nextId = 4;
    }

    public void seed() {
        kreirajTabeluPredmetaAkoNePostoji();
        popuniPredmete();
    }

    public ObservableList<Predmet> dajSvePredmete() {
        List<Predmet> predmeti = new ArrayList<>();
        String upit = "SELECT * FROM predmeti";

        try (Connection conn = connect();
             Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(upit)) {

            while (rs.next()) {
                Predmet p = new Predmet(
                        rs.getInt("id"),
                        rs.getString("naziv"),
                        rs.getDouble("ECTS")
                );
                predmeti.add(p);
            }
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
        return FXCollections.observableArrayList(predmeti);
    }

    public Predmet dajPredmetPoId(Integer id) {
        Predmet predmet = null;
        String upit = "SELECT * FROM predmeti WHERE id = ?";
        try (Connection conn = connect();
             PreparedStatement pstmt = conn.prepareStatement(upit)) {
            pstmt.setInt(1, id);
            ResultSet rs = pstmt.executeQuery();
            if (rs.next()) {
                return new Predmet(
                        rs.getInt("id"),
                        rs.getString("naziv"),
                        rs.getDouble("ECTS")
                );
            }
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
        return predmet;
    }

    public void dodajPredmet(String naziv, Double ECTS) {
        Predmet predmet = new Predmet(nextId, naziv, ECTS);
        String upit = "INSERT INTO predmeti (naziv, ECTS) VALUES (?, ?)";

        try (Connection conn = connect();
             PreparedStatement pstmt = conn.prepareStatement(upit)) {
            pstmt.setString(1, naziv);
            pstmt.setDouble(2, ECTS);
            pstmt.executeUpdate();
            System.out.println("Dodat novi red u tabelu.");
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
    }

    public void azurirajPredmet(Predmet predmet) {
        String upit = "UPDATE predmeti SET naziv = ?, ECTS = ? WHERE id = ?";

        try (Connection conn = connect();
             PreparedStatement pstmt = conn.prepareStatement(upit)) {
            pstmt.setString(1, predmet.getNaziv());
            pstmt.setDouble(2, predmet.getECTS());
            pstmt.setInt(3, predmet.getId());
            int brojAzuriranihRedova = pstmt.executeUpdate();
            System.out.println("Azuriran red u tabeli. Broj azuriranih redova: " + brojAzuriranihRedova);
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
    }

    public void obrisiPredmetPoId(Integer id) {
        String upit = "DELETE FROM predmeti WHERE id = ?";

        try (Connection conn = connect();
             PreparedStatement pstmt = conn.prepareStatement(upit)) {
            pstmt.setInt(1, id);
            int brojObrisanihRedova = pstmt.executeUpdate();
            System.out.println("Obrisan red iz tabele. Broj obrisanih redova: " + brojObrisanihRedova);
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
    }

    public void obrisiSvePredmete() {
        String upit = "DELETE FROM predmeti";

        try (Connection conn = connect();
             Statement stmt = conn.createStatement()) {
            int brojObrisanihRedova = stmt.executeUpdate(upit);
            System.out.println("Obrisani redovi tabele. Broj obrisanih redova: " + brojObrisanihRedova);
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
    }
}
