package com.example.z2.controller;


import com.example.z2.model.Predmet;
import com.example.z2.repository.PredmetRepository;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.control.ListView;
import javafx.scene.control.TextField;

public class PredmetController {
    private PredmetRepository predmetRepository;
    private ObservableList<Predmet> predmetiObservableList = FXCollections.observableArrayList();

    @FXML
    public ListView<Predmet> predmetiListView;

    @FXML
    public TextField nazivInput;
    @FXML
    public TextField ectsInput;

    @FXML
    public Label responseText;

    @FXML
    public void initialize() {
        predmetRepository = new PredmetRepository();
        predmetRepository.seed();
        predmetiObservableList.addAll(predmetRepository.dajSvePredmete());
        predmetiListView.setItems(predmetiObservableList);
    }

    @FXML
    public void onDodajPredmet() {
        String naziv = nazivInput.getText();
        try {
            Double ects = Double.parseDouble(ectsInput.getText());

            try {
                predmetRepository.dodajPredmet(naziv, ects);
                predmetiObservableList.clear();
                predmetiObservableList.addAll(predmetRepository.dajSvePredmete());
                responseText.styleProperty().set("-fx-text-fill: green");
                responseText.setText("Uspjesno dodan predmet!");
                predmetiObservableList.forEach(System.out::println);
                nazivInput.setText("");
                ectsInput.setText("");
            } catch (IllegalArgumentException e) {
                responseText.styleProperty().set("-fx-text-fill: red");
                responseText.setText(e.getMessage());
            } catch (Exception e) {
                responseText.styleProperty().set("-fx-text-fill: red");
                responseText.setText("Doslo je do greske prilikom dodavanja predmeta");
            }
        } catch (NumberFormatException e) {
            responseText.styleProperty().set("-fx-text-fill: red");
            responseText.setText("ECTS mora biti broj!");
        }
    }

}
