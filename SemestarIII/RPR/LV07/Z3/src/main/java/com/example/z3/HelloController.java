package com.example.z3;

import com.example.z3.controller.PredmetController;
import com.example.z3.model.Predmet;
import com.example.z3.view.PredmetView;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;

public class HelloController {
    public TextField nazivInput;
    public TextField ectsInput;
    private PredmetView view;
    private Predmet predmet;
    private PredmetController controller;

    @FXML
    private void initialize() {
        predmet = new Predmet("Razvoj programskih rješenja", 5.0);
        view = new PredmetView();
        controller = new PredmetController(predmet, view);
        displayPredmet();
    }

    @FXML
    private Label predmetNazivText;
    @FXML
    private Label predmetECTSText;
    @FXML
    private Label responseText;

    protected void displayPredmet() {
        controller.dajPredmet();
        String naziv = view.getPoruka();
        controller.dajECTS();
        String ECTS = view.getPoruka();
        predmetNazivText.setText(naziv);
        predmetECTSText.setText(ECTS);
    }

    @FXML
    protected void onAzurirajNazivButtonClick() {
        String naziv = nazivInput.getText();
        if(naziv.isEmpty()) {
            responseText.setText("Naziv predmeta ne moze biti prazan");
            return;
        }
        controller.azurirajPredmet(naziv);
        responseText.setText(view.getPoruka());
        displayPredmet();
    }
    @FXML
    protected void onAzurirajECTSButtonClick() {
        String ects = ectsInput.getText();
        if(ects.isEmpty()) {
            responseText.setText("ECTS predmeta ne moze biti prazan");
            return;
        }
        controller.azurirajECTS(Double.parseDouble(ects));
        responseText.setText(view.getPoruka());
        displayPredmet();
    }
}