#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <sys/types.h>
#include <utility>
#include <vector>

class Vector {
    std::vector<double> elementi;
public:
    explicit Vector(int n) { 
        if(n <= 0) throw std::range_error("Bad dimension");
        elementi = std::vector<double>(n, 0); 
    }
    Vector(std::initializer_list<double> l) {
        if(l.size() <= 0) throw std::range_error("Bad dimension");
        elementi = std::vector<double>(l);
    }
    int NElems() const { return elementi.size(); };
    double &operator[](int i) { return elementi[i]; };
    double operator[](int i) const { return elementi[i]; };
    double &operator()(int i) {
        if(i < 1 || i > NElems()) throw std::range_error("Invalid index");
        return elementi[i-1];
    }
    double operator()(int i) const {
        if(i < 1 || i > NElems()) throw std::range_error("Invalid index");
        return elementi[i-1];
    };
    double Norm() const {
        double s = 0;
        std::for_each(elementi.begin(), elementi.end(), [&s](double x){s += x*x;});
        return std::sqrt(s);
    };
    friend double VectorNorm(const Vector &v);
    double GetEpsilon() const {
        return 10 * Norm() * std::numeric_limits<double>::epsilon();
    }
    void Print(char separator = '\n', double eps = -1) const {
        if(eps < 0) eps = GetEpsilon();
        for(int i = 0; i < NElems(); i++) {
            if(std::fabs(elementi[i]) < eps)
                std::cout << 0;
            else std::cout << elementi[i];
            if(i != NElems()-1 || separator == '\n') std::cout << separator;
        }
    }
    friend void PrintVector(const Vector &v, char separator, double eps);
    friend Vector operator +(const Vector &v1, const Vector &v2);
    Vector &operator +=(const Vector &v) {
        if(v.NElems() != NElems()) throw std::domain_error("Incompatible formats");
        for(int i = 0; i < NElems(); i++)
            elementi[i] += v[i];
        return *this;
    }
    friend Vector operator -(const Vector &v1, const Vector &v2);
    Vector &operator -=(const Vector &v) {
        if(v.NElems() != NElems()) throw std::domain_error("Incompatible formats");
        for(int i = 0; i < NElems(); i++)
            elementi[i] -= v[i];
        return *this;
    }
    friend Vector operator *(double s, const Vector &v);
    friend Vector operator *(const Vector &v, double s);
    Vector &operator *=(double s) {
        for(int i = 0; i < NElems(); i++)
            elementi[i] *= s;
        return *this;
    }
    friend double operator *(const Vector &v1, const Vector &v2);
    friend Vector operator /(const Vector &v, double s);
    Vector &operator /=(double s) {
        if(std::fabs(s) <= std::numeric_limits<double>::epsilon()) throw std::domain_error("Division by zero");
        for(int i = 0; i < NElems(); i++)
            elementi[i] /= s;
        return *this;
    }
};

Vector operator +(const Vector &v1, const Vector &v2) {
    if(v1.NElems() != v2.NElems()) throw std::domain_error("Incompatible formats");
    Vector rezultat(v1.NElems());
    for(int i = 0; i < rezultat.NElems(); i++)
        rezultat[i] = v1[i] + v2[i];
    return rezultat;
}

Vector operator -(const Vector &v1, const Vector &v2) {
    if(v1.NElems() != v2.NElems()) throw std::domain_error("Incompatible formats");
    Vector rezultat(v1.NElems());
    for(int i = 0; i < rezultat.NElems(); i++)
        rezultat[i] = v1[i] - v2[i];
    return rezultat;
}

Vector operator *(double s, const Vector &v) {
    Vector rezultat(v.NElems());
    for(int i = 0; i < v.NElems(); i++)
        rezultat[i] = v[i];
    std::for_each(rezultat.elementi.begin(), rezultat.elementi.end(), [&s](double &x){x*=s;});
    return rezultat;
}

Vector operator *(const Vector &v, double s) { return s * v; }

double operator *(const Vector &v1, const Vector &v2) {
    if(v1.NElems() != v2.NElems()) throw std::domain_error("Incompatible formats");
    double s = 0;
    for(int i = 0; i < v1.NElems(); i++)
        s += v1[i] * v2[i];
    return s;
}

Vector operator /(const Vector &v, double s) {
    if(std::fabs(s) <= std::numeric_limits<double>::epsilon()) throw std::domain_error("Division by zero");
    Vector rezultat(v.NElems());
    for(int i = 0; i < v.NElems(); i++)
        rezultat[i] = v[i];
    std::for_each(rezultat.elementi.begin(), rezultat.elementi.end(), [&s](double &x){x/=s;});
    return rezultat;
}

void PrintVector(const Vector &v, char separator = '\n', double eps = -1) {
    if(eps < 0) eps = v.GetEpsilon();
    for(int i = 0; i < v.elementi.size(); i++) {
        if(std::fabs(v[i]) < eps)
            std::cout << 0;
        else std::cout << v[i];
        if(i != v.NElems()-1 || separator == '\n') std::cout << separator;
    }
}

double VectorNorm(const Vector &v) {
    double s = 0;
    std::for_each(v.elementi.begin(), v.elementi.end(), [&s](double x){s += x*x;});
    return std::sqrt(s);
}


// ======================== MATRIX ========================

class Matrix {
    std::vector<std::vector<double>> elementi;
public:
    Matrix(int m, int n) {
        if(m <= 0 || n <= 0) throw std::range_error("Bad dimension");
        elementi = std::vector<std::vector<double>>(m, std::vector<double>(n, 0));
    }
    Matrix(const Vector &v) {
        elementi = std::vector<std::vector<double>>(v.NElems(), std::vector<double>(1, 0));
        for(int i = 0; i < v.NElems(); i++) elementi[i][0] = v[i];
    }
    Matrix(std::initializer_list<std::initializer_list<double>> l) {
        if(l.size() == 0) throw std::range_error("Bad dimension");
        int vel = l.begin()->size();
        if(vel == 0) throw std::range_error("Bad dimension");
        for(auto v : l) if(v.size() != vel) throw std::logic_error("Bad matrix");
        elementi = std::vector<std::vector<double>>();
        for(auto v : l) elementi.push_back(std::vector<double>(v));
    }
    int NRows() const { return elementi.size(); };
    int NCols() const { return elementi[0].size(); };
    double *operator[](int i) { return elementi[i].data(); };
    const double *operator[](int i) const { return elementi[i].data(); };
    double &operator()(int i, int j) {
        if(i < 1 || i > NRows() || j < 1 || j > NCols()) throw std::range_error("Invalid index");
        return elementi[i-1][j-1];
    }
    double operator()(int i, int j) const {
        if(i < 1 || i > NRows() || j < 1 || j > NCols()) throw std::range_error("Invalid index");
        return elementi[i-1][j-1];
    }
    double Norm() const {
        double s = 0;
        for(auto v : elementi)
            for(auto el : v)
                s+=el*el;
        return std::sqrt(s);
    }
    friend double MatrixNorm(const Matrix &m);
    double GetEpsilon() const {
        return 10 * Norm() * std::numeric_limits<double>::epsilon();
    };
    void Print(int width = 10, double eps = -1) const {
        if(eps < 0) eps = GetEpsilon();
        for(int i = 0; i < elementi.size(); i++) {
            for(int j = 0; j < elementi[i].size(); j++) {
                if(std::fabs(elementi[i][j]) < eps)
                    std::cout << 0;
                else std::cout << std::setw(width) << elementi[i][j];
            }
            std::cout << '\n';
        }
    }
    friend void PrintMatrix(const Matrix &m, int width, double eps);
    friend Matrix operator +(const Matrix &m1, const Matrix &m2);
    Matrix &operator +=(const Matrix &m) {
        if(NRows() != m.NRows() || NCols() != m.NCols()) throw std::domain_error("Incompatible formats");
        for(int i = 0; i < NRows(); i++)
            for(int j = 0; j < NCols(); j++)
                (*this)[i][j] += m[i][j];
        return *this;
    }
    friend Matrix operator -(const Matrix &m1, const Matrix &m2);
    Matrix &operator -=(const Matrix &m) {
        if(NRows() != m.NRows() || NCols() != m.NCols()) throw std::domain_error("Incompatible formats");
        for(int i = 0; i < NRows(); i++)
            for(int j = 0; j < NCols(); j++)
                (*this)[i][j] -= m[i][j];
        return *this;
    }
    friend Matrix operator *(double s, const Matrix &m);
    friend Matrix operator *(const Matrix &m, double s);
    Matrix &operator *=(double s) {
        for(int i = 0; i < NRows(); i++)
            for(int j = 0; j < NCols(); j++)
                (*this)[i][j] *= s;
        return *this;
    }
    friend Matrix operator *(const Matrix &m1, const Matrix &m2);
    Matrix &operator *=(const Matrix &m) {
        if(NCols() != m.NRows()) throw std::domain_error("Incompatible formats");
        Matrix rezultat(NRows(), m.NCols());
        for(int i = 0; i < NRows(); i++) {
            for(int j = 0; j < m.NCols(); j++) {
                double s = 0;
                for(int k = 0; k < m.NRows(); k++)
                    s += (*this)[i][k] * m[k][j];
                rezultat[i][j] = s;
            }
        }
        elementi = rezultat.elementi;
        return *this;
    }
    friend Vector operator *(const Matrix &m, const Vector &v);
    friend Matrix Transpose(const Matrix &m);
    void Transpose() {
        if(NRows() == NCols()) {
            for(int i = 0; i < NRows(); i++)
                for(int j = 0; j < i; j++)
                    std::swap(elementi[i][j], elementi[j][i]);
            return;
        }
        Matrix transponovana(NCols(), NRows());
        for(int i = 0; i < NRows(); i++)
            for(int j = 0; j < NCols(); j++)
                transponovana[j][i] = (*this)[i][j];
        elementi = transponovana.elementi;
    }
};

Matrix Transpose(const Matrix &m) {
    Matrix transponovana(m.NCols(), m.NRows());
    for(int i = 0; i < m.NRows(); i++)
        for(int j = 0; j < m.NCols(); j++)
            transponovana[j][i] = m[i][j];
    return transponovana;
}

Vector operator *(const Matrix &m, const Vector &v) {
    if(m.NCols() != v.NElems()) throw std::domain_error("Incompatible formats");
    Vector rezultat(m.NRows());
    for(int i = 0; i < m.NRows(); i++) {
        double s = 0;
        for(int j = 0; j < v.NElems(); j++)
            s+=m[i][j] * v[j];
        rezultat[i] = s;
    }
    return rezultat;
}

Matrix operator *(const Matrix &m1, const Matrix &m2) {
    if(m1.NCols() != m2.NRows()) throw std::domain_error("Incompatible formats");
    Matrix rezultat(m1.NRows(), m2.NCols());
    for(int i = 0; i < m1.NRows(); i++) {
        for(int j = 0; j < m2.NCols(); j++) {
            double s = 0;
            for(int k = 0; k < m2.NRows(); k++)
                s += m1[i][k] * m2[k][j];
            rezultat[i][j] = s;
        }
    }
    return rezultat;
}

Matrix operator *(const Matrix &m, double s) { return s * m; }

Matrix operator *(double s, const Matrix &m) {
    Matrix rezultat(m.NRows(), m.NCols());
    for(int i = 0; i < m.NRows(); i++)
        for(int j = 0; j < m.NCols(); j++)
            rezultat[i][j] = s*m[i][j];
    return rezultat;
}
Matrix operator -(const Matrix &m1, const Matrix &m2) {
    if(m1.NRows() != m2.NRows() || m1.NCols() != m2.NCols()) throw std::domain_error("Incompatible formats");
    Matrix rezultat(m1.NRows(), m1.NCols());
    for(int i = 0; i < m1.NRows(); i++)
        for(int j = 0; j < m1.NCols(); j++)
            rezultat[i][j] = m1[i][j] - m2[i][j];
    return rezultat;
}
Matrix operator +(const Matrix &m1, const Matrix &m2) {
    if(m1.NRows() != m2.NRows() || m1.NCols() != m2.NCols()) throw std::domain_error("Incompatible formats");
    Matrix rezultat(m1.NRows(), m1.NCols());
    for(int i = 0; i < m1.NRows(); i++)
        for(int j = 0; j < m1.NCols(); j++)
            rezultat[i][j] = m1[i][j] + m2[i][j];
    return rezultat;
}

double MatrixNorm(const Matrix &m) {
    double s = 0;
    for(int i = 0; i < m.NRows(); i++) 
        for(int j = 0; j < m.NCols(); j++) 
            s += m[i][j] * m[i][j];
    return std::sqrt(s);
}
void PrintMatrix(const Matrix &m, int width = 10, double eps = -1) {
    if(eps < 0) eps = m.GetEpsilon();
    for(int i = 0; i < m.elementi.size(); i++) {
        for(int j = 0; j < m.elementi[i].size(); j++) {
            if(std::fabs(m[i][j]) < eps)
                std::cout << 0;
            else std::cout << std::setw(width) << m[i][j];
        }
        std::cout << '\n';
    }
}

/*void NR() { std::cout << "\n"; }

void TestiranjeKonstruktora() {
    std::cout << "Testiranje konstruktora" << std::endl;
    Vector v0(5);
    try {
        Vector v1(0);
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Vector v1{};
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    Vector v2{1, 2, 3, 4, 5};
    try {
        Vector v4(0);
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Vector v5{1, 2, 3, 4, 5};
        Vector v6 = v5;
        v6[0] = 10;
        std::cout << v5[0] << std::endl;
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Vector v7{1, 2, 3, 4, 5};
        Vector v8(5);
        v8 = v7;
        v8[0] = 10;
        std::cout << v7[0] << std::endl;
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    Vector v9{1, 2, 3, 4, 5};
    std::cout << v9.NElems();
    NR();
}

void TestiranjeOperatoraUglaste() {
    std::cout << "Testiranje operator[] i operator()" << std::endl;
    Vector v1{1, 2, 3, 4, 5};
    std::cout << v1[0] << std::endl;
    std::cout << v1(1) << std::endl;
    try {
        std::cout << v1[5] << std::endl;
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        std::cout << v1(6) << std::endl;
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        std::cout << v1[0] << std::endl;
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        std::cout << v1(1) << std::endl;
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        std::cout << v1[-1] << std::endl;
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        std::cout << v1(-1) << std::endl;
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        std::cout << v1[200] << std::endl;
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        std::cout << v1(100) << std::endl;
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
}

void TestiranjeNormeVektora() {
    std::cout << "Testiranje norme i epsilona vektora" << std::endl;
    Vector v1{1, 2, 3, 4, 5};
    std::cout << v1.Norm() << std::endl;
    std::cout << VectorNorm(v1) << std::endl;
    Vector v2{-8, -7, -1, -1, 13};
    std::cout << v2.Norm() << std::endl;
    std::cout << VectorNorm(v2) << std::endl;
    std::cout << v1.GetEpsilon() << std::endl;
    std::cout << v2.GetEpsilon() << std::endl;
}

void TestiranjePrintanjaVektora() {
    std::cout << "Testiranje printanja vektora" << std::endl;
    Vector v1{1, 2, 3, 4, 5};
    v1.Print();
    NR();
    v1.Print(' ');
    NR();
    v1.Print('s', 2);
    NR();
    v1.Print('-', 0.01);
    NR();
    PrintVector(v1, '#', 3);
    NR();
}

void TestiranjeOperacijaNadVektorima() {
    std::cout << "Testiranje operacija nad vektorima" << std::endl;
    Vector v1{1, 2, 3, 4, 5};
    Vector v2{5, 4, 3, 2, 1};
    Vector v3 = v1 + v2;
    v3.Print(' ');
    NR();
    v3 = v1 - v2;
    v3.Print(' ');
    NR();
    v3 = 2 * v1;
    v3.Print(' ');
    NR();
    v3 = v1 * 2;
    v3.Print(' ');
    NR();
    v3 = v1 / 2;
    v3.Print(' ');
    NR();
    double s = v1 * v2;
    std::cout << s << std::endl;
    try {
        Vector v4{1, 2, 3, 4, 5};
        Vector v5{5, 4, 3, 2};
        Vector v6 = v4 + v5;
        v6.Print(' ');
        NR();
    } catch(std::domain_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Vector v4{1, 2, 3, 4, 5};
        Vector v5{5, 4, 3, 2};
        Vector v6 = v4 - v5;
        v6.Print(' ');
        NR();
    } catch(std::domain_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Vector v4{1, 2, 3, 4, 5};
        Vector v5{5, 4, 3, 2};
        double a = v4 * v5;
    } catch(std::domain_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Vector v4{1, 2, 3, 4, 5};
        Vector v5{5, 4, 3, 2};
        Vector v6 = v4 / 0;
        v6.Print(' ');
        NR();
    } catch(std::domain_error e) {
        std::cout << e.what() << std::endl;
    }
    Vector v4{1, 2, 3, 4, 5};
    Vector v5{5, 4, 3, 2, 1};
    v4 += v5;
    v4.Print(' ');
    NR();
    v4 -= v5;
    v4.Print(' ');
    NR();
    v4 *= 2;
    v4.Print(' ');
    NR();
    v4 /= 2;
    v4.Print(' ');
    NR();
}

void TestiranjeKonstruktoraMatrice() {
    std::cout << "Testiranje konstruktora matrice" << std::endl;
    Matrix m1(5, 5);
    try {
        Matrix m2(0, 0);
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Matrix m3(0, 5);
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Matrix m4(5, 0);
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Matrix m5{};
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Matrix m6{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    } catch(std::logic_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Matrix m7{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        Matrix m8 = m7;
        m8[0][0] = 10;
        std::cout << m7[0][0] << std::endl;
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Matrix m9{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        Matrix m10(3, 3);
        m10 = m9;
        m10[0][0] = 10;
        std::cout << m9[0][0] << std::endl;
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Matrix m11{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        std::cout << m11[0][0] << std::endl;
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
}

void TestiranjeOperatoraUglasteZaMatricu() {
    std::cout << "Testiranje operator[] i operator()" << std::endl;
    Matrix m{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    try {
        std::cout << m[2][2] << std::endl;
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        std::cout << m(4, 4) << std::endl;
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        std::cout << m[0][0] << std::endl;
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        std::cout << m(2, 2) << std::endl;
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        std::cout << m(-1, -1) << std::endl;
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        std::cout << m(100, 100) << std::endl;
    } catch(std::range_error e) {
        std::cout << e.what() << std::endl;
    }
}

void TestiranjeNormeMatrice() {
    std::cout << "Testiranje norme matrice" << std::endl;
    Matrix m{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    std::cout << m.Norm() << std::endl;
    std::cout << MatrixNorm(m) << std::endl;
    Matrix m1{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    std::cout << m1.GetEpsilon() << std::endl;
}

void TestiranjePrintanjaMatrice() {
    std::cout << "Testiranje printanja matrice" << std::endl;
    Matrix m{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    m.Print();
    NR();
    m.Print(5);
    NR();
    m.Print(5, 0.01);
    NR();
    PrintMatrix(m, 5, 0.01);
    NR();
}

void TestiranjeOperacijaNadMatricama() {
    std::cout << "Testiranje operacija nad matricama" << std::endl;
    Matrix m1{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    Matrix m2{{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    Matrix m3 = m1 + m2;
    m3.Print();
    NR();
    m3 = m1 - m2;
    m3.Print();
    NR();
    m3 = 2 * m1;
    m3.Print();
    NR();
    m3 = m1 * 2;
    m3.Print();
    NR();
    Matrix s = m1 * m2;
    s.Print();
    try {
        Matrix m4{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        Matrix m5{{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
        Matrix m6 = m4 + m5;
        m6.Print();
        NR();
    } catch(std::domain_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Matrix m4{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        Matrix m5{{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
        Matrix m6 = m4 - m5;
        m6.Print();
        NR();
    } catch(std::domain_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Matrix m4{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        Matrix m5{{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
        Matrix a = m4 * m5;
    }
    catch(std::domain_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Matrix m4{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        Matrix m5{{9, 8, 7}, {6, 5, 4}};
        Matrix m6 = m4 + m5;
        m6.Print();
        NR();
    } catch(std::domain_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Matrix m4{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        Matrix m5{{9, 8, 7}, {6, 5, 4}};
        Matrix m6 = m4 - m5;
        m6.Print();
        NR();
    } catch(std::domain_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Matrix m4{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        Matrix m5{{9, 8, 7}, {6, 5, 4}};
        Matrix a = m4 * m5;
    }
    catch(std::domain_error e) {
        std::cout << e.what() << std::endl;
    }
    Matrix m4{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    Matrix m5{{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    m4 += m5;
    m4.Print();
    NR();
    m4 -= m5;
    m4.Print();
    NR();
    m4 *= 2;
    m4.Print();
    NR();
    try {
        Matrix m4{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        Matrix m5{{9, 8, 7}, {6, 5, 4}};
        m4 += m5;
        m4.Print();
        NR();
    } catch(std::domain_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Matrix m4{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        Matrix m5{{9, 8, 7}, {6, 5, 4}};
        m4 -= m5;
        m4.Print();
        NR();
    } catch(std::domain_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Matrix m4{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        m4 *= 0;
        m4.Print();
        NR();
    } catch(std::domain_error e) {
        std::cout << e.what() << std::endl;
    }
    try {
        Matrix m4{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        Matrix m5{{1, 2}, {4, 5, 6}, {7, 8, 9}};
        m4 *= m5;
        m4.Print();
        NR();
    } catch(std::domain_error e) {
        std::cout << e.what() << std::endl;
    } catch(std::logic_error e) {
        std::cout << e.what() << std::endl;
    }
}

int main() {
    TestiranjeKonstruktora();
    TestiranjeOperatoraUglaste();
    TestiranjeNormeVektora();
    TestiranjePrintanjaVektora();
    TestiranjeOperacijaNadVektorima();
    TestiranjeKonstruktoraMatrice();
    TestiranjeOperatoraUglasteZaMatricu();
    TestiranjeNormeMatrice();
    TestiranjePrintanjaMatrice();
    TestiranjeOperacijaNadMatricama();
    return 0;
}*/
