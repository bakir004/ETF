#include <iostream>
#include <stdexcept>
#include <string>
#include <array>

// zadatak radjen uz 10. predavanje
class Student {
    static const int brojOcjena = 10;
    std::string ime, prezime;
    std::array<int,brojOcjena> ocjene;
    int indeks;
    int godinaStudija;
public:
    int DajGodinaStudija() { return godinaStudija; }
    int DajIndeks() { return indeks; }
    std::array<int,brojOcjena> DajOcjene() { return ocjene; }
    std::string DajPrezime() { return prezime; }
    std::string DajIme() { return ime; }
    Student(): ime(""), prezime(""), ocjene({0,0,0,0,0,0,0,0,0,0}), indeks(0), godinaStudija(0) {}
    int DajOcjenu(int redniBrojKursa) const {
        if(redniBrojKursa < 1 || redniBrojKursa > brojOcjena)
            throw std::domain_error("Nekorektni parametri");
        return ocjene[redniBrojKursa-1];
    }
    void PostaviStudenta(std::string ime, std::string prezime, int indeks, int godinaStudija, std::array<int, 10> ocjene) {
        if(indeks < 0) throw std::domain_error("Indeks ne smije biti negativan");
        this->ime = ime;
        this->prezime = prezime;
        this->indeks = indeks;
        this->godinaStudija = godinaStudija;
        this->ocjene = ocjene;
    } 
    void PostaviOcjenu(int redniBrojKursa, int ocjena) {
        if(redniBrojKursa < 1 || redniBrojKursa > brojOcjena || ocjena < 5 || ocjena > 10) 
            throw std::domain_error("Nekorektni parametri");
        ocjene[redniBrojKursa-1] = ocjena;
    }
    double DajProsjek() const {
        double prosjek = 0;
        int brojPetica = 0;
        for(auto ocjena : ocjene) {
            if(ocjena > 5) prosjek += ocjena;
            else brojPetica++;
        }
        if(brojPetica == brojOcjena) return 5;
        else return prosjek/brojOcjena;
    }
    bool ImaLiProlaz() const {
        bool vecPaoJedan = false;
        for(auto ocjena : ocjene) {
            if(ocjena == 5 && vecPaoJedan) return false;
            if(ocjena == 5) vecPaoJedan = true;
        }
        return true;
    }
    void PrednjiNaSljedecuGodinu() {
        for(auto &ocjena : ocjene) ocjena = 5;
        godinaStudija++;
    }
    bool DaLiJeBoljiOd(const Student &drugi) const {
        return DajProsjek() < drugi.DajProsjek();
    }
    static bool DaLiJePrviBolji(const Student &s1, const Student &s2) {
        return s1.DajProsjek() > s2.DajProsjek();
    }
    void Ispisi() const {
        std::cout << "Student " << ime << " " << prezime << " s brojem indeksa " << indeks << " " << godinaStudija << ". godine studija ima ocjene:\n";
        for(int i = 0; i < ocjene.size(); i++)
            std::cout << ocjene[i] << (i == brojOcjena - 1 ? "" : ", ");
        std::cout << "\n";
    }
};

int main() {
    Student s1, s2;
    s1.PostaviStudenta("Bakir", "Cinjarevic", 19705, 1, {10,10,9,7,7,7,8,7,7,6});
    s2.PostaviStudenta("Zakir", "Cinjarevic", 19706, 2, {10,8,9,5,6,5,6,7,7,6});
    s1.Ispisi();
    std::cout << s1.DajProsjek() << "\n";
    std::cout << s2.DajProsjek() << "\n";
    std::cout << s2.ImaLiProlaz() << "\n";
    std::cout << s1.DaLiJeBoljiOd(s2) << "\n";
    s2.PostaviOcjenu(4, 10);
    s2.PostaviOcjenu(6, 10);
    std::cout << s2.DajProsjek() << "\n";
    std::cout << Student::DaLiJePrviBolji(s1, s2);
    return 0;
}
