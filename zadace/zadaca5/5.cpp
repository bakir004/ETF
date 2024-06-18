#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iomanip>

template <typename TipEl>
class Matrica {
    int br_redova, br_kolona;
    TipEl **elementi;
    char ime_matrice;
    static TipEl **AlocirajMemoriju(int br_redova, int br_kolona);
    static void DealocirajMemoriju(TipEl **elementi, int br_redova);
    void KopirajElemente(TipEl **elementi);
public:
    Matrica(int br_redova, int br_kolona, char ime = 0);
    Matrica(const Matrica &m);
    Matrica(Matrica &&m);
    Matrica(std::string ime, bool bin);
    ~Matrica() { DealocirajMemoriju(elementi, br_redova); }
    Matrica &operator =(const Matrica &m);
    Matrica &operator =(Matrica &&m);
    template <typename Tip2>
        friend std::istream &operator >>(std::istream &tok, Matrica<Tip2> &m);
    template <typename Tip2>
        friend std::ostream &operator <<(std::ostream &tok, const Matrica<Tip2> &m);
    template <typename Tip2>
        friend Matrica<Tip2> operator +(const Matrica<Tip2> &m1, const Matrica<Tip2> &m2);
    template <typename Tip2>
        friend Matrica<Tip2> operator -(const Matrica<Tip2> &m1, const Matrica<Tip2> &m2);
    template <typename Tip2>
        friend Matrica<Tip2> operator *(const Matrica<Tip2> &m1, const Matrica<Tip2> &m2);
    template <typename Tip2>
        friend Matrica<Tip2> operator *(const Matrica<Tip2> &m, Tip2 d);
    template <typename Tip2>
        friend Matrica<Tip2> operator *(Tip2 d, const Matrica<Tip2> &m);
    Matrica &operator +=(const Matrica &m);
    Matrica &operator -=(const Matrica &m);
    Matrica &operator *=(const Matrica &m);
    Matrica &operator *=(TipEl d);
    TipEl *operator [](int i) { return elementi[i]; }
    const TipEl *operator [](int i) const { return elementi[i]; }
    TipEl &operator ()(int i, int j);
    const TipEl& operator()(int i, int j) const;
    operator std::string() const;
    void SacuvajUTekstualnuDatoteku(std::string);
    void SacuvajUBinarnuDatoteku(std::string);
    void ObnoviIzTekstualneDatoteke(std::string);
    void ObnoviIzBinarneDatoteke(std::string);
};
template <typename TipEl>
Matrica<TipEl>::Matrica(std::string ime, bool bin): elementi(nullptr) {
    if(!bin) ObnoviIzTekstualneDatoteke(ime);
    else ObnoviIzBinarneDatoteke(ime);
}
template <typename TipEl>
TipEl ** Matrica<TipEl>::AlocirajMemoriju(int br_redova, int br_kolona) {
    TipEl **elementi = new TipEl*[br_redova]{};
    try {
        for(int i = 0; i < br_redova; i++) elementi[i] = new TipEl[br_kolona];
    }
    catch(...) {
        DealocirajMemoriju(elementi, br_redova);
        throw;
    }
    return elementi;
}
template <typename TipEl>
void Matrica<TipEl>::DealocirajMemoriju(TipEl **elementi, int br_redova) {
    for(int i = 0; i < br_redova; i++) delete[] elementi[i];
    delete[] elementi;
}
template <typename TipEl>
Matrica<TipEl>::Matrica(int br_redova, int br_kolona, char ime) :
    br_redova(br_redova), br_kolona(br_kolona), ime_matrice(ime),
    elementi(AlocirajMemoriju(br_redova, br_kolona)) {}
    template <typename TipEl>
    void Matrica<TipEl>::KopirajElemente(TipEl **elementi) {
        for(int i = 0; i < br_redova; i++)
            for(int j = 0; j < br_kolona; j++) Matrica::elementi[i][j] = elementi[i][j];
    }
template <typename TipEl>
Matrica<TipEl>::Matrica(const Matrica &m) : br_redova(m.br_redova),
    br_kolona(m.br_kolona), ime_matrice(m.ime_matrice),
    elementi(AlocirajMemoriju(m.br_redova, m.br_kolona)) {
        KopirajElemente(m.elementi);
    }
template <typename TipEl>
Matrica<TipEl>::Matrica(Matrica &&m) : br_redova(m.br_redova), br_kolona(m.br_kolona),
    elementi(m.elementi), ime_matrice(m.ime_matrice) {
        m.br_redova = 0; m.elementi = nullptr;
    }
template <typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator =(const Matrica &m) {
    if(br_redova < m.br_redova || br_kolona < m.br_kolona) {
        TipEl **novi_prostor = AlocirajMemoriju(m.br_redova, m.br_kolona);
        DealocirajMemoriju(elementi, br_redova);
        elementi = novi_prostor;
    }
    else if(br_redova > m.br_redova)
        for(int i = m.br_redova; i < br_redova; i++) delete elementi[i];
    br_redova = m.br_redova; br_kolona = m.br_kolona;
    ime_matrice = m.ime_matrice;
    KopirajElemente(m.elementi);
    return *this;
}
template <typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator =(Matrica &&m) {
    std::swap(br_redova, m.br_redova); std::swap(br_kolona, m.br_kolona);
    std::swap(ime_matrice, m.ime_matrice); std::swap(elementi, m.elementi);
    return *this;
}
template <typename TipEl>
std::istream &operator >>(std::istream &tok, Matrica<TipEl> &m) {
    for(int i = 0; i < m.br_redova; i++)
        for(int j = 0; j < m.br_kolona; j++) {
            if(&tok == &std::cin)
                std::cout << m.ime_matrice << "(" << i + 1 << "," << j + 1 << ") = ";
            tok >> m.elementi[i][j];
        }
    return tok;
}
template <typename TipEl>
std::ostream &operator <<(std::ostream &tok, const Matrica<TipEl> &m) {
    int sirina_ispisa = tok.width();
    for(int i = 0; i < m.br_redova; i++) {
        for(int j = 0; j < m.br_kolona; j++)
            std::cout << std::setw(sirina_ispisa) << m.elementi[i][j];
        std::cout << std::endl;
    }
    return tok;
}
template <typename TipEl>
Matrica<TipEl> operator +(const Matrica<TipEl> &m1, const Matrica<TipEl> &m2) {
    if(m1.br_redova != m2.br_redova || m1.br_kolona != m2.br_kolona)
        throw std::domain_error("Matrice nemaju jednake dimenzije!");
    Matrica<TipEl> m3(m1.br_redova, m1.br_kolona);
    for(int i = 0; i < m1.br_redova; i++)
        for(int j = 0; j < m1.br_kolona; j++)
            m3.elementi[i][j] = m1.elementi[i][j] + m2.elementi[i][j];
    return m3;
}
template <typename TipEl>
Matrica<TipEl> operator -(const Matrica<TipEl> &m1, const Matrica<TipEl> &m2) {
    if(m1.br_redova != m2.br_redova || m1.br_kolona != m2.br_kolona)
        throw std::domain_error("Matrice nemaju jednake dimenzije!");
    Matrica<TipEl> m3(m1.br_redova, m1.br_kolona);
    for(int i = 0; i < m1.br_redova; i++)
        for(int j = 0; j < m1.br_kolona; j++)
            m3.elementi[i][j] = m1.elementi[i][j] - m2.elementi[i][j];
    return m3;
}
template <typename TipEl>
Matrica<TipEl> operator *(const Matrica<TipEl> &m1, const Matrica<TipEl> &m2) {
    if(m1.br_kolona != m2.br_redova)
        throw std::domain_error("Matrice nisu saglasne za mnozenje");
    Matrica<TipEl> m3(m1.br_redova, m2.br_kolona);
    for(int i = 0; i < m1.br_redova; i++)
        for(int j = 0; j < m2.br_kolona; j++) {
            m3.elementi[i][j] = TipEl();
            for(int k = 0; k < m1.br_kolona; k++)
                m3.elementi[i][j] += m1.elementi[i][k] * m2.elementi[k][j];
        }
    return m3;
}
template <typename TipEl>
Matrica<TipEl> operator *(const Matrica<TipEl> &m, TipEl d) {
    Matrica<TipEl> m2(m.br_redova, m.br_kolona);
    for(int i = 0; i < m.br_redova; i++)
        for(int j = 0; j < m.br_kolona; j++)
            m2.elementi[i][j] = m.elementi[i][j] * d;
    return m2;
}
template <typename TipEl>
inline Matrica<TipEl> operator *(TipEl d, const Matrica<TipEl> &m) {
    return m * d;
}
template <typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator +=(const Matrica &m) {
    if(br_redova != m.br_redova || br_kolona != m.br_kolona)
        throw std::domain_error("Matrice nemaju jednake dimenzije!");
    for(int i = 0; i < br_redova; i++)
        for(int j = 0; j < br_kolona; j++)
            elementi[i][j] += m.elementi[i][j];
    return *this;
}
template <typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator -=(const Matrica &m) {
    if(br_redova != m.br_redova || br_kolona != m.br_kolona)
        throw std::domain_error("Matrice nemaju jednake dimenzije!");
    for(int i = 0; i < br_redova; i++)
        for(int j = 0; j < br_kolona; j++)
            elementi[i][j] -= m.elementi[i][j];
    return *this;
}
template <typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator *=(const Matrica &m) {
    if(br_kolona != m.br_redova)
        throw std::domain_error("Matrice nisu saglasne za mnozenje");
    Matrica<TipEl> m1 = *this;
    for(int i = 0; i < br_redova; i++)
        for(int j = 0; j < m.br_kolona; j++) {
            elementi[i][j] = TipEl();
            for(int k = 0; k < br_kolona; k++)
                elementi[i][j] += m1.elementi[i][k] * m.elementi[k][j];
        }
    return *this;
}
template <typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator *=(TipEl d) {
    for(int i = 0; i < br_redova; i++)
        for(int j = 0; j < br_kolona; j++) elementi[i][j] *= d;
    return *this;
}
template <typename TipEl>
TipEl &Matrica<TipEl>::operator ()(int i, int j) {
    if(i < 1 || i > br_redova || j < 1 || j > br_kolona)
        throw std::range_error("Neispravan indeks");
    return elementi[i - 1][j - 1];
}
template <typename TipEl>
const TipEl &Matrica<TipEl>::operator ()(int i, int j) const {
    if(i < 1 || i > br_redova || j < 1 || j > br_kolona)
        throw std::range_error("Neispravan indeks");
    return elementi[i - 1][j - 1];
}
template <typename TipEl>
Matrica<TipEl>::operator std::string() const {
    using std::to_string;
    std::string s = "{";
    for(int i = 0; i < br_redova; i++) {
        s += "{";
        for(int j = 0; j < br_kolona; j++) {
            s += to_string(elementi[i][j]);
            if(j != br_kolona - 1) s += ",";
        }
        s += "}";
        if(i != br_redova - 1) s += ",";
    }
    return s += "}";
}

template <typename TipEl>
void Matrica<TipEl>::SacuvajUTekstualnuDatoteku(std::string ime_datoteke) {
    std::ofstream izlazni_tok(ime_datoteke);
    for(int i = 0; i < br_redova; i++) {
        for(int j = 0; j < br_kolona; j++) {
            izlazni_tok << elementi[i][j];       
            if(j != br_kolona - 1) izlazni_tok << ',';
            if(izlazni_tok.bad()) throw std::logic_error("Problemi sa upisom u datoteku");
        }
        izlazni_tok << '\n';
    }
}
template <typename TipEl>
void Matrica<TipEl>::SacuvajUBinarnuDatoteku(std::string ime_datoteke) {
    std::ofstream izlazni_tok(ime_datoteke, std::ios::binary);
    izlazni_tok.write(reinterpret_cast<char*>(this), sizeof *this);
    for(int i = 0; i < br_redova; i++) {
        for(int j = 0; j < br_kolona; j++) {
            if(!izlazni_tok.write(reinterpret_cast<char*>(&elementi[i][j]), sizeof (TipEl)))
                throw std::logic_error("Problemi sa upisom u datoteku");
        }
    }
}

template <typename TipEl>
void Matrica<TipEl>::ObnoviIzTekstualneDatoteke(std::string ime_datoteke) {
    std::ifstream ulazni_tok(ime_datoteke);
    if(!ulazni_tok) throw std::logic_error("Trazena datoteka ne postoji");
    br_redova = 0, br_kolona = -1;
    int trenutno = 0;
    while(ulazni_tok) {
        TipEl n;
        char c;
        ulazni_tok >> n;
        trenutno++;
        if(ulazni_tok.peek() == '\n') {
            br_redova++;
            if(br_kolona != -1 && trenutno != br_kolona) throw std::logic_error("Datoteka sadrzi besmislene podatke");
            br_kolona = trenutno;
            trenutno = 0;
            if(ulazni_tok.peek() == EOF) break;
            ulazni_tok.ignore();
            continue;
        }
        ulazni_tok >> c;
        if(ulazni_tok.bad()) throw std::logic_error("Problemi pri citanju iz datoteke");
        if(ulazni_tok.eof()) break;
        if(c != ',') throw std::logic_error("Datoteka sadrzi besmislene podatke");
    }
    if(elementi != nullptr)
        DealocirajMemoriju(elementi, br_redova);
    elementi = AlocirajMemoriju(br_redova, br_kolona);
    ulazni_tok.clear();
    ulazni_tok.seekg(0, std::ios::beg);
    int i = 0;
    bool citajC = false;
    std::string red;
    while(std::getline(ulazni_tok, red)) {
        std::istringstream tok(red);
        char c;
        TipEl n;
        while(tok >> n) {
            elementi[i / br_kolona][i % br_kolona] = n;
            i++;

            if (tok >> c) {
                if (c != ',') throw std::logic_error("Datoteka sadrzi besmislene podatke");
            } else if (!tok.eof())
                throw std::logic_error("Datoteka sadrzi besmislene podatke");
        }
    }
}

template <typename TipEl>
void Matrica<TipEl>::ObnoviIzBinarneDatoteke(std::string ime_datoteke) {
    std::ifstream ulazni_tok(ime_datoteke, std::ios::binary);
    if(!ulazni_tok) throw std::logic_error("Trazena datoteka ne postoji");
    if(elementi != nullptr)
        DealocirajMemoriju(elementi, br_redova);
    ulazni_tok.read(reinterpret_cast<char*>(this), sizeof *this);
    elementi = AlocirajMemoriju(br_redova, br_kolona);
    TipEl* niz = new TipEl[br_redova * br_kolona];
    if(!ulazni_tok.read(reinterpret_cast<char*>(niz),
                br_kolona * br_redova * sizeof(TipEl))) {
        DealocirajMemoriju(elementi, br_redova);
        delete[] niz;
        throw std::logic_error("Problemi sa upisom u datoteku");
    }
    for(int i = 0; i < br_redova; i++)
        for(int j = 0; j < br_kolona; j++)
            elementi[i][j] = niz[i*br_kolona+j];
    delete[] niz;
}

int main() {
    const std::string filename = "file.txt";
    Matrica<double> m(3, 4);
    m(1,1) = 2.5; m(1,2) = -3; m(1,3) = 1.12; m(1,4) = 4;
    m(2,1) = 0; m(2,2) = 0.25; m(2,3) = 3.16; m(2,4) = 42.3;
    m(3,1) = -1.7; m(3,2) = 2.5; m(3,3) = 0; m(3,4) = 4;
    m.SacuvajUTekstualnuDatoteku(filename);

    char data[] = "Hello, world!";
    std::ofstream file(filename, std::ios::app);
    file.write(reinterpret_cast<const char*>(data), sizeof(data));

    try
    {
        Matrica<double> m2(filename, false);
        std::cout << m2;
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << '\n';
    }
}























