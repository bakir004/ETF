module com.example.z3 {
    requires javafx.controls;
    requires javafx.fxml;

    requires org.controlsfx.controls;
    requires org.kordamp.bootstrapfx.core;

    opens com.example.z3 to javafx.fxml;
    exports com.example.z3;
}