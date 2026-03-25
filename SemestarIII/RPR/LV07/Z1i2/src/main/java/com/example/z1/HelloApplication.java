package com.example.z1;

import com.example.z1.controller.OsobaController;
import com.example.z1.model.OsobaModel;
import com.example.z1.view.OsobaView;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;

public class HelloApplication extends Application {
    @Override
    public void start(Stage stage) throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(HelloApplication.class.getResource("hello-view.fxml"));
        Scene scene = new Scene(fxmlLoader.load(), 320, 240);
        stage.setTitle("Hello!");
        stage.setScene(scene);
        stage.show();
    }

    public static void main(String[] args) {
        OsobaModel osobaModel = new OsobaModel();
        osobaModel.napuni();

        OsobaView osobaView = new OsobaView();
        osobaView.setUlazniTekst("Novo ime");

        OsobaController osobaController = new OsobaController(osobaModel, osobaView);
        osobaController.azurirajIme(1);

        System.out.println("1) View ispisuje: " + osobaView.getPoruka());
        System.out.println("   Azurirana osoba je: " + osobaController.dajOsobuPoId(1).toString());

        osobaController.dajOsobeIzTxtDatoteke("src/data/osobe.txt");
        System.out.println("2) View ispisuje: " + osobaView.getPoruka());

        osobaController.dajOsobeIzXmlDatoteke("src/data/osobe.xml");
        System.out.println("3) View ispisuje: " + osobaView.getPoruka());
//        launch();
    }
}