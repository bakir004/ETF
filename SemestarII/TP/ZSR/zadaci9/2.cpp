#include <cmath>
#include <iostream>

class Robot {
    double x = 0, y = 0, ugao = 0;
public:
    void IdiNaprijed(double pomak) {
        this->x += pomak*std::sin(this->ugao);
        this->y += pomak*std::cos(this->ugao);
    }
    void IdiNazad(double pomak) {
        this->x -= pomak*std::sin(this->ugao);
        this->y -= pomak*std::cos(this->ugao);
    }
    void Postavi(double x, double y, double ugao) {
        this->x = x;
        this->y = y;
        this->ugao = ugao;
    }
    void OkreniSeNalijevo(double ugao) { this->ugao += ugao; }
    void OkreniSeNadesno(double ugao) { this->ugao -= ugao; }
    double DajPozicijuX() const { return this->x; }
    double DajPozicijuY() const { return this->y; }
    double DajOrijentaciju() const { return this->ugao; }
    void Ispisi() const {
        std::cout << "Robot se nalazi na poziciji (" << x << "," << y << ") i gleda pod uglom od " << (ugao / 3.14159) * 180 << " stepeni u odnosu na x-osu\n";
    }
};


int main() {
    Robot robot;
    robot.Ispisi();
    robot.OkreniSeNalijevo(3.14159/2);
    robot.IdiNaprijed(1);
    robot.Ispisi();
    robot.IdiNazad(2);
    robot.Ispisi();
    return 0;
}
