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
    Matrix &operator /=(double s) {
        if(std::fabs(s) == 0) throw std::domain_error("Division by zero");
        for(int i = 0; i < NRows(); i++)
            for(int j = 0; j < NCols(); j++)
                elementi[i][j] /= s;
        return *this;
    }
    friend Matrix operator /(Matrix m1, Matrix m2);
    Matrix &operator /=(Matrix m) {
        Matrix res = *this / m;
        this->elementi = res.elementi;
        return *this;
    }
    double Det() const {
        if(NCols() != NRows()) throw std::domain_error("Matrix is not square");
        Matrix m1 = *this;
        int n = NRows();
        double det = 1;
        for(int k = 0; k < n; k++) {
            int p = k;
            for(int i = k+1; i < n; i++)
                if(std::fabs(m1.elementi[i][k]) > std::fabs(m1.elementi[p][k])) p = i;
            if(std::fabs(m1[p][k]) < m1.GetEpsilon()) return 0;
            if(p != k) {
                std::swap(m1.elementi[p], m1.elementi[k]);
                det = -det;
            }
            det *= m1[k][k];
            for(int i = k+1; i < n; i++) {
                double mi = m1[i][k] / m1[k][k];
                for(int j = k+1; j < n; j++)
                    m1[i][j] -= mi * m1[k][j];
            }
        }   
        return det;
    }
    friend double Det(Matrix m);
    void Invert() {
        int n = NRows();
        if(NCols() != NRows()) throw std::domain_error("Matrix is not square");
        std::vector<int> w(NRows(), 0);
        for(int k = 0; k < n; k++) {
            int p = k;
            for(int i = k+1; i < n; i++)
                if(std::fabs(elementi[i][k]) > std::fabs(elementi[p][k])) p = i;
            if(std::fabs(elementi[p][k]) < GetEpsilon()) throw std::domain_error("Matrix is singular");
            if(p != k) 
                std::swap(elementi[p], elementi[k]);
            w[k] = p;
            double mi = elementi[k][k];
            elementi[k][k] = 1;
            for(int j = 0; j < n; j++)
                elementi[k][j] /= mi;
            for(int i = 0; i < n; i++)
                if(i != k) {
                    mi = elementi[i][k];
                    elementi[i][k] = 0;
                    for(int j = 0; j < n; j++)
                        elementi[i][j] -= mi * elementi[k][j];
                }
        }       
        for(int j = n-1; j >= 0; j--) {
            int p = w[j];
            if(p != j)
                for(int i = 0; i < n; i++)
                    std::swap(elementi[i][p], elementi[i][j]);
        }
    }
    friend Matrix Inverse(Matrix m);
    void ReduceToRREF() {
        int m = NRows();
        int n = NCols();
        std::vector<bool> w(n, false);
        int k = -1, l = -1, p;
        while(k < m && l < n) {
            l++;
            k++;
            double v = 0;
            while(v < GetEpsilon() && l < n) {
                p = k;
                for(int i = k; i < m; i++) {
                    if(std::fabs(elementi[i][l]) > v) {
                        v = std::fabs(elementi[i][l]);
                        p = i;
                    }
                }
                if(v < GetEpsilon())
                    l++;
            }
            if(l < n) {
                w[l] = true;
                if(p != k)
                    std::swap(elementi[p], elementi[k]);
                double mi = elementi[k][l];
                for(int j = l; j < n; j++)
                    elementi[k][j] /= mi;
                for(int i = 0; i < m; i++) {
                    if(i != k) {
                        mi = elementi[i][l];
                        for(int j = l; j < n; j++) {
                            elementi[i][j] -= mi*elementi[k][j];
                        }
                    }
                }
            }
        }
    }
    friend Matrix RREF(Matrix m);
    int Rank() const {
        Matrix rref = RREF(*this);
        int rank = 0;
        for(int i = 0; i < rref.NCols(); i++) {
            int sum = 0;
            bool bad = false;
            for(int j = 0; j < rref.NRows(); j++) {
                if(rref[j][i] != 0 && rref[j][i] != 1) {
                    bad = true;
                    break;
                }
                else sum += rref[j][i];
            }
            if(bad) continue;
            if(sum == 1) rank++;
        }
        return rank;
    }
    friend int Rank(Matrix m);
};
// svjestan sam duplog kopiranja, draze mi je ovako nego dodavat jos koda
int Rank(Matrix m) { return m.Rank(); }
Matrix RREF(Matrix m) { m.ReduceToRREF(); return m; }
Matrix Inverse(Matrix m) { m.Invert(); return m; }
double Det(Matrix m) { return m.Det(); }

Matrix operator /(const Matrix &m, double s) {
    if(std::fabs(s) == 0) throw std::domain_error("Division by zero");
    Matrix rezultat(m.NRows(), m.NCols());
    for(int i = 0; i < m.NRows(); i++)
        for(int j = 0; j < m.NCols(); j++)
            rezultat[i][j] = m[i][j] / s;
    return rezultat;
}

Matrix operator /(Matrix m1, Matrix m2) {
    // ne pitajte
    std::swap(m1, m2);
    if(m1.NCols() != m1.NRows()) throw std::domain_error("Divisor matrix is not square");
    if(m1.NCols() != m2.NCols()) throw std::domain_error("Incompatible formats");
    Matrix x(m2.NRows(), m1.NRows());
    int n = m1.NRows();
    int m = m2.NRows();
    for(int k = 0; k < n; k++) {
        int p = k;
        for(int i = k+1; i < n; i++)
            if(std::fabs(m1.elementi[k][i]) > std::fabs(m1.elementi[k][p])) p = i;
        if(std::fabs(m1[k][p]) < m1.GetEpsilon()) throw std::domain_error("Divisor matrix is singular");
        // razmjena kolona
        if(p != k) {
            for (int i = 0; i < n; i++)
                std::swap(m1[i][p], m1[i][k]);
            for (int i = 0; i < m; i++)
                std::swap(m2[i][p], m2[i][k]);
        }
        for(int i = k+1; i < n; i++) {
            double mi = m1[k][i] / m1[k][k];
            for(int j = k+1; j < n; j++)
                m1[j][i] -= mi * m1[j][k];
            for(int j = 0; j < m; j++)
                m2[j][i] -= mi * m2[j][k];
        }
    }   
    for(int k = 0; k < m; k++) {
        for(int i = n-1; i >= 0; i--) {
            double s = m2[k][i];
            for(int j = i+1; j < n; j++)
                s -= m1[j][i] * x[k][j];
            x[k][i] = s / m1[i][i];
        }
    }
    return x;
}
Vector LeftDiv(Matrix m, Vector v) {
    Matrix m2 = Matrix(v);
    Matrix res = LeftDiv(m, m2);
    for(int i = 0; i < res.NRows(); i++)
        v[i] = res[i][0];
    return v;
}


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
        // razmjena redova
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

class LUDecomposer {
    Matrix LU;
    Vector w;
public:
    LUDecomposer(Matrix m): LU(m.NRows(), m.NRows()), w(m.NRows()) {
        if(m.NRows() != m.NCols()) throw std::domain_error("Matrix is not square");
        int n = m.NCols();
        for(int i = 0; i < n; i++)
            w[i] = i;
        for(int j = 0; j < n; j++) {
            for(int i = 0; i <= j; i++) {
                double s = m[i][j];
                for(int k = 0; k < i; k++)
                    s -= m[i][k]*m[k][j];
                m[i][j] = s;
            }
            int p = j;
            for(int i = j+1; i < n; i++) {
                double s = m[i][j];
                for(int k = 0; k < j; k++)
                    s -= m[i][k]*m[k][j];
                m[i][j] = s;
                if(std::fabs(s) > std::fabs(m[p][j]))
                    p = i;
            }
            if(std::fabs(m[p][j]) < m.GetEpsilon())
                throw std::domain_error("Matrix is singular");
            if(p != j) {
                for(int k = 0; k < n; k++)
                    std::swap(m[p][k], m[j][k]);
            }
            w[j]=p;
            double mi = m[j][j];
            for(int i = j+1; i < n; i++)
                m[i][j] /= mi;
        }
        LU = m;
    }
    void Solve(const Vector &b, Vector &x) const {
        if(LU.NCols() != x.NElems() || LU.NRows() != b.NElems()) throw std::domain_error("Incompatible formats");
        Vector b1 = b;
        for (int i = 0; i < b1.NElems(); i++) {
            int p = w[i];
            double s = b1[p];
            b1[p] = b1[i];
            for (int j = 0; j < i; j++)
                s -= LU[i][j] * x[j];
            x[i] = s;
        }
        for (int i = b1.NElems() - 1; i >= 0; i--) {
            double s = x[i];
            for (int j = i + 1; j < b1.NElems(); j++)
                s -= LU[i][j] * x[j]; 
            x[i] = s / LU[i][i]; 
        }
    }
    Vector Solve(Vector b) const {
        if(LU.NRows() != b.NElems()) throw std::domain_error("Incompatible formats");
        Vector x(b.NElems());
        for (int i = 0; i < b.NElems(); i++) {
            int p = w[i];
            double s = b[p];
            b[p] = b[i];
            for (int j = 0; j < i; j++)
                s -= LU[i][j] * x[j];
            x[i] = s;
        }
        for (int i = b.NElems() - 1; i >= 0; i--) {
            double s = x[i];
            for (int j = i + 1; j < b.NElems(); j++)
                s -= LU[i][j] * x[j]; 
            x[i] = s / LU[i][i]; 
        }
        return x;
    }
    void Solve(const Matrix &b, Matrix &x) const {
        if(x.NCols() != b.NCols()) throw std::domain_error("Incompatible formats");
        if(x.NRows() != LU.NRows()) throw std::domain_error("Incompatible formats");
        if(LU.NCols() != x.NCols() || LU.NRows() != b.NRows()) throw std::domain_error("Incompatible formats");
        for(int i = 0; i < b.NCols(); i++) {
            Vector bCol(b.NRows());
            for(int j = 0; j < b.NRows(); j++)
                bCol[j] = b[j][i];
            Vector xCol = Solve(bCol);
            for(int j = 0; j < x.NRows(); j++)
                x[j][i] = xCol[j];
        }
    }
    Matrix Solve(Matrix b) const {
        Matrix x(b.NRows(), b.NCols());
        Solve(b, x);
        return x;
    }
    Matrix GetCompactLU() const { return LU; }
    Matrix GetL() const {
        Matrix L(LU.NRows(), LU.NCols());
        for(int i = 0; i < L.NRows(); i++) {
            for(int j = 0; j < L.NCols(); j++) {
                if(j < i)
                    L[i][j] = LU[i][j];
                else if(i == j)
                    L[i][j] = 1;
                else
                    L[i][j] = 0;
            }
        }
        return L;
    }
    Matrix GetU() const {
        Matrix U(LU.NRows(), LU.NCols());
        for(int i = 0; i < U.NRows(); i++) {
            for(int j = 0; j < U.NCols(); j++) {
                if(j >= i)
                    U[i][j] = LU[i][j];
                else
                    U[i][j] = 0;
            }
        }
        return U;
    }
    Vector GetPermuation() const { return w; }
};

class QRDecomposer {
    Matrix V;
    Matrix R;
public:
    QRDecomposer(Matrix m): V(m.NRows(), m.NCols()), R(m.NRows(), m.NCols()) {
        if(m.NRows() < m.NCols()) throw std::domain_error("Invalid matrix format");
        Matrix v(m.NRows(), m.NCols());
        for(int k = 0; k < m.NCols(); k++) {
            double s = 0;
            for(int i = k; i < m.NRows(); i++)
                s += m[i][k] * m[i][k];
            s = std::sqrt(s);
            double mi = std::sqrt(s*(s+std::fabs(m[k][k])));
            if(m[k][k] < 0) s = -s;

            v[k][k] = (m[k][k] + s) / mi;
            for(int i = k+1; i < m.NRows(); i++)
                v[i][k] = m[i][k] / mi; 
            m[k][k] = -s;
            for(int j = k+1; j < m.NCols(); j++) {
                s = 0;
                for(int i = k; i < m.NRows(); i++)
                    s += m[i][j] * v[i][k];
                for(int i = k; i < m.NRows(); i++)
                    m[i][j] -= s * v[i][k];
            }
        }
        V = v;
        R = m;
    }
    void Solve(const Vector &b, Vector &x) const {

    }
    Vector Solve(Vector b) const;
    void Solve(Matrix &b, Matrix &x) const;
    Matrix Solve(Matrix b) const;
    Vector MulQWith(Vector v) const;
    Matrix MulQWith(Matrix m) const;
    Vector MulQTWith(Vector v) const;
    Matrix MulQTWith(Matrix m) const;
    Matrix GetQ() const {
        Matrix Q(V.NRows(), V.NCols());
        for(int j = 0; j < V.NRows(); j++) {
            for(int i = 0; i < V.NRows(); i++)
                Q[i][j] = 0;
            Q[j][j] = 1;
            for(int k = V.NCols()-1; k >= 0; k--) {
                double s = 0;
                for(int i = k; i < V.NRows(); i++)
                    s += V[i][k] * Q[i][j];
                for(int i = k; i < V.NRows(); i++)
                    Q[i][j] -= s * V[i][k];
            }
        }
        return Q;
    }
    Matrix GetR() const { 
        Matrix r(R);
        for(int i = 0; i < r.NRows(); i++)
            for(int j = 0; j < i; j++)
                r[i][j] = 0;
        return r; 
    }
};


int main() {
    Matrix a{{1, 2, 4}, {4, 5, 6}, {7, 8, 9}};
    QRDecomposer qrd(a);
    Matrix q = qrd.GetQ();
    Matrix r = qrd.GetR();
    PrintMatrix(q);
    PrintMatrix(r);
}
