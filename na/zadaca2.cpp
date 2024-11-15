#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
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

    // ZADACA 2
    void Chop(double eps = -1) {
        if(eps < 0) eps = GetEpsilon();
        std::for_each(elementi.begin(), elementi.end(), [&eps](double &x){if(std::fabs(x) < eps) x = 0;});
    }
    bool EqualTo(const Vector& v, double eps = -1) const {
        if(eps < 0) eps = GetEpsilon();
        if(NElems() != v.NElems()) return false;
        for(int i = 0; i < NElems(); i++)
            if(std::fabs(elementi[i] - v[i]) > eps) return false;
        return true;
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
                    std::cout << std::setw(width) << 0;
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

    // ZADACA 2
    
    void Chop(double eps = -1) {
        if(eps < 0) eps = GetEpsilon();
        for(int i = 0; i < NRows(); i++)
            for(int j = 0; j < NCols(); j++)
                if(std::fabs(elementi[i][j]) < eps)
                    elementi[i][j] = 0;
    }
    bool EqualTo(const Matrix &m, double eps = -1) const {
        if(eps < 0) eps = GetEpsilon();
        if(NRows() != m.NRows() || NCols() != m.NCols()) return false;
        for(int i = 0; i < NRows(); i++)
            for(int j = 0; j < NCols(); j++)
                if(std::fabs(elementi[i][j] - m[i][j]) > eps) return false;
        return true;
    }
    friend Matrix LeftDiv(Matrix m1, Matrix m2);
    friend Vector LeftDiv(Matrix m, Vector v);
    friend Matrix operator /(const Matrix &m, double s);
    Matrix &operator /=(double s);
    friend Matrix operator /(Matrix m1, Matrix m2);
    Matrix &operator /=(Matrix m);
    double Det() const;
    friend double Det(Matrix m);
    void Invert();
    friend Matrix Inverse(Matrix m);
    void ReduceToRREF();
    friend Matrix RREF(Matrix m);
    int Rank() const;
    friend int Rank(Matrix m);
};

/*Matrix operator/(const Matrix &m, double s) {*/
/**/
/*}*/
/*Vector LeftDiv(Matrix m, Vector v) {*/
/*}*/
Matrix LeftDiv(Matrix m1, Matrix m2) {
    if(m1.NCols() != m1.NRows()) throw std::domain_error("Divisor matrix is not square");
    if(m1.NRows() != m2.NRows()) throw std::domain_error("Incompatible formats");
    Matrix x(m1.NCols(), m2.NCols());
    int n = m1.NRows();
    int m = m2.NCols();
    for(int k = 0; k < n; k++) {
        int p = k;
        for(int i = k+1; i < n; i++)
            if(std::fabs(m1.elementi[i][k]) > std::fabs(m1.elementi[p][k])) p = i;
        if(std::fabs(m1[p][k]) < m1.GetEpsilon()) throw std::domain_error("Divisor matrix is singular");
        if(p != k) {
            std::swap(m1.elementi[p], m1.elementi[k]);
            std::swap(m2.elementi[p], m2.elementi[k]);
        }
        for(int i = k+1; i < n; i++) {
            double mi = m1[i][k] / m1[k][k];
            for(int j = k+1; j < n; j++)
                m1[i][j] -= mi * m1[k][j];
            for(int j = 0; j < m; j++)
                m2[i][j] -= mi * m2[k][j];
        }
    }   
    for(int k = 0; k < m; k++) {
        for(int i = n-1; i >= 0; i--) {
            double s = m2[i][k];
            for(int j = i+1; j < n; j++)
                s -= m1[i][j] * x[j][k];
            x[i][k] = s / m1[i][i];
        }
    }
    return x;
}

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
                std::cout << std::setw(width) << 0;
            else std::cout << std::setw(width) << m[i][j];
        }
        std::cout << '\n';
    }
}

void NR() { std::cout << "\n"; }

int main() {
    Matrix a{{1, 2, 4},{4, 5, 6},{7, 8, 9}};
    Matrix b{{1, 2, 7}, {4, 5, 6}, {7, 8, 9}};
    // a.Print(3);
    // b.Print(3);
    Matrix rez = LeftDiv(a, b);
    rez.Print(3);
}
