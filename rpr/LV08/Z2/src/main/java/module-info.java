module com.example.z2 {
    requires javafx.controls;
    requires javafx.fxml;

    requires org.controlsfx.controls;
    requires org.kordamp.bootstrapfx.core;

    opens com.example.z2 to javafx.fxml;
    opens com.example.z2.controller to javafx.fxml; // Open the controller package
    exports com.example.z2;
}