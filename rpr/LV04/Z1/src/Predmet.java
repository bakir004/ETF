import java.util.List;

public class Predmet implements IInformacije
{
    private String naziv;
    private Profesor odgovorniProfesor;
    private List<Asistent> asistenti;
    private Double ECTS;

    public Predmet(String naziv, Profesor profesor, List<Asistent> asistenti, Double ects)
    {
        this.naziv = naziv;
        this.odgovorniProfesor = profesor;
        this.asistenti = asistenti;
        this.ECTS = ects;
    }

    public String DajInformacije()
    {
        return "Predmet: " + naziv + ", odgovorni profesor: " + odgovorniProfesor.DajInformacije();
    }
}