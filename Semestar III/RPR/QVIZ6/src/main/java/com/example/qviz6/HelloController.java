package com.example.qviz6;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.control.ListView;

import java.util.ArrayList;
import java.util.List;

public class HelloController {
    @FXML
    private ObservableList<Integer> lista = FXCollections.observableArrayList();

    @FXML
    private ListView<Integer> listElement;

    @FXML
    private void initialize() {
        lista.addAll(List.of(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
        listElement.setItems(lista);
    }

    @FXML
    protected void parniClick() {
        List<Integer> parni = new ArrayList<>();
        for (Integer i : lista) {
            if (i % 2 == 0) {
                parni.add(i);
            }
        }
        listElement.setItems(FXCollections.observableArrayList(parni));
    }
    @FXML
    protected void neparniClick() {
        List<Integer> neparni = new ArrayList<>();
        for (Integer i : lista) {
            if (i % 2 == 1) {
                neparni.add(i);
            }
        }
        listElement.setItems(FXCollections.observableArrayList(neparni));
    }
    @FXML
    protected void sviClick() {
        listElement.setItems(lista);
    }
}