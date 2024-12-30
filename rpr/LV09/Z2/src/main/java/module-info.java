module com.example.z2 {
    requires javafx.controls;
    requires javafx.fxml;

    requires org.controlsfx.controls;
    requires org.kordamp.bootstrapfx.core;
    requires java.sql;

    opens com.example.z2 to javafx.fxml;
    opens com.example.z2.controller to javafx.fxml;
    exports com.example.z2;
}