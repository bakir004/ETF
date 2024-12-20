#include <iostream>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <algorithm>
#define EPSILON 0.00000001

typedef std::pair<double, double> Par;
typedef std::vector<Par> Dataset;
typedef std::vector<double> VectorDouble;

const double PI = std::atan(1)*4;

class AbstractInterpolator {
    mutable int lastAccessedInterval;
protected:
    Dataset data;
    int Locate(double x) const {
        if(data.at(lastAccessedInterval-1).first < x && x <= data[lastAccessedInterval-1].first) 
            return lastAccessedInterval;
        // treba susjedne provjerit
        if(x > data.at(data.size()-1).first) return data.size();
        if(x < data.at(0).first) return 0;
        auto p = std::lower_bound(data.begin(), data.end(), Par(x,0), [](Par p1, Par p2){
            return p1.first < p2.first;
        });
        int razlika = p-data.begin();
        return razlika;
    }
public:
    AbstractInterpolator(const Dataset &data): lastAccessedInterval(1) {
        this->data = data;
        std::sort(this->data.begin(), this->data.end(), [](const Par& p1, const Par& p2){
            if(p1.first == p2.first) 
                throw std::domain_error("Invalid data set");
            return p1.first < p2.first;
        });
    }
    virtual double operator()(double x) const = 0;
    void PrintDataset() {
        std::cout << "[";
        for(int i = 0; i < data.size(); i++)
            std::cout << "" << data[i].first << ", " << data[i].second << "; ";
        std::cout << "]\n";
    }
};

class LinearInterpolator : public AbstractInterpolator {
public:
    LinearInterpolator(const std::vector<std::pair<double, double>> &data): AbstractInterpolator(data) {}
    double operator()(double x) const override {
        int foundInterval = Locate(x), i;
        if(foundInterval == 0) i = 0;
        else if(foundInterval == data.size()) i = data.size()-2;
        else i = --foundInterval;

        return data.at(i).second*(data.at(i+1).first-x)/(data.at(i+1).first-data.at(i).first)
            + data.at(i+1).second*(x-data.at(i).first)/(data.at(i+1).first-data.at(i).first);
    }
};

class PolynomialInterpolator : public AbstractInterpolator {
    VectorDouble q;
    VectorDouble y;
public:
    PolynomialInterpolator(const std::vector<std::pair<double, double>> &data): AbstractInterpolator(data), q(data.size()), y(data.size()) {
        int n = this->data.size();
        for(int i = 0; i < n; i++)
            q.at(i) = this->data.at(i).second;

        y.at(0) = data[n-1].second;
        for(int j = 1; j < n; j++) {
            for(int i = n-1; i>=j; i--) {
                if(i == n-1) y.at(j) = (q.at(i)-q.at(i-1))/(data.at(i).first-data.at(i-j).first);
                q.at(i) = (q.at(i)-q.at(i-1))/(data.at(i).first-data.at(i-j).first);
            }
        }
    }
    double operator()(double x) const override {
        int n = data.size();

        double f = q.at(n-1);
        for(int i = n-1; i >= 1; i--)
            f = f*(x-data.at(i-1).first)+q.at(i-1);
        return f;
    }
    void AddPoint(const Par &p) {
        for(int i = 0; i < data.size(); i++)
            if(p.first == data.at(i).first) throw std::domain_error("Invalid point");
        data.push_back(p);
        int n = data.size();
        VectorDouble novi(y.size()+1);
        novi.at(0) = p.second;
        for(int i = 1; i < n-1; i++)
            novi.at(i) = (novi.at(i-1) - y.at(i-1))/(data.at(n-1).first-data.at(n-1-i).first);
        novi.at(n-1)=(novi.at(n-2)-y.at(n-2))/(p.first - data.at(0).first);
        y = novi;
        q.push_back(y.at(y.size()-1));
    }
    VectorDouble GetCoefficients() const { 
        int n = data.size();
        VectorDouble w(n+1), v(n+1), p(n);
        for(int j = 0; j < n; j++)
            p.at(j) = 0;
        w.at(0)=1;
        for(int i = 1; i <= n; i++) {
            w.at(i) = w.at(i-1);
            for(int j = i-1; j >= 1; j--)
                w.at(j) = w.at(j-1)-data.at(i-1).first * w.at(j);
            w.at(0) = -data.at(i-1).first*w.at(0);
        }
        for(int i = 0; i < n; i++) {
            double alpha = 1;
            for(int j = 0; j < n; j++)
                if(i != j)
                    alpha *= (data.at(i).first - data.at(j).first);
            alpha = data.at(i).second/alpha;
            for(int j = 0; j <= n; j++)
                v.at(j) = w.at(j);
            for(int j = n-1; j >= 0; j--) {
                v.at(j) += data.at(i).first * v.at(j+1);
                p.at(j) += alpha * v.at(j+1);
            }
        }
        return p;
    };
};

class PiecewisePolynomialInterpolator : public AbstractInterpolator {
    int k;
    int clamp(int min, int max, int target) const {
        if(target < min) return min;
        if(target > max) return max;
        return target;
    }
public:
    PiecewisePolynomialInterpolator(const std::vector<std::pair<double, double>> &data, int order): AbstractInterpolator(data) {
        if(order <= 1 || order >= data.size()) throw std::domain_error("Invalid order");
        k = order;
    }
    double operator()(double x) const override {
        int foundInterval = Locate(x);
        int n = data.size();
        int i = clamp(k/2+1, n-(k/2+1), foundInterval)-1;
        Dataset subset;
        if(k % 2 == 1) {
            for(int j = i-(k-1)/2; j <= i+(k+1)/2; j++)
                subset.push_back(data.at(j));
        } else {
            for(int j = i-k/2; j <= i+k/2; j++)
                subset.push_back(data.at(j));
        }
        n = subset.size();
        double s = 0;
        for(int i = 0; i < n; i++) {
            double p = subset.at(i).second;
            for(int j = 0; j < n; j++)
                if(j != i)
                    p *= (x-subset.at(j).first)/(subset.at(i).first-subset.at(j).first);
            s += p;
        }
        return s;
    }
};

class SplineInterpolator : public AbstractInterpolator {
    VectorDouble r, q, s;
    int clamp(int min, int max, int target) const {
        if(target < min) return min;
        if(target > max) return max;
        return target;
    }
public:
    SplineInterpolator(const std::vector<std::pair<double, double>> &dataUnsorted): AbstractInterpolator(dataUnsorted), r(data.size()), s(data.size()), q(data.size()) {
        int n = data.size();
        r.at(0) = 0;
        r.at(n-1) = 0;
        VectorDouble alpha(n);
        for(int i = 1; i <= n-2; i++) {
            alpha[i] = 2*(data.at(i+1).first-data.at(i-1).first);
            r[i] = 3*((data.at(i+1).second-data.at(i).second)/(data.at(i+1).first-data.at(i).first)-
                      (data.at(i).second-data.at(i-1).second)/(data.at(i).first-data.at(i-1).first));
        }
        for(int i = 1; i < n-2; i++) {
            double mi = (data.at(i+1).first - data.at(i).first)/alpha.at(i);
            alpha.at(i+1) -= mi*(data.at(i+1).first-data.at(i).first);
            r.at(i+1) -= mi*r.at(i);
        }
        r.at(n-2)/=alpha.at(n-2);
        for(int i = n-3; i >= 1; i--)
            r.at(i) = (r.at(i)-(data.at(i+1).first-data.at(i).first)*r.at(i+1))/alpha.at(i);

        for(int i = 0; i < n-1; i++) {
            double deltaX = data.at(i+1).first - data.at(i).first;
            s.at(i) = (r.at(i+1)-r.at(i))/(3*deltaX);
            q.at(i) = (data.at(i+1).second-data.at(i).second)/deltaX - deltaX*(r.at(i+1)+2*r.at(i))/3;
        }
    }
    double operator()(double x) const override {
        int n = data.size();
        int foundInterval = Locate(x);
        int i = clamp(1, data.size()-1, foundInterval)-1;
        double t = x-data[i].first;
        return data[i].second + t *(q[i] + t*(r[i] + t*s[i]));
    }
};

class BarycentricInterpolator : public AbstractInterpolator {
    VectorDouble w;
    int d;
public:
    BarycentricInterpolator(const std::vector<std::pair<double, double>> &data, int order): AbstractInterpolator(data) {
        if(order < 0 || order > data.size()) throw std::domain_error("Invalid order");
        int n = data.size();
        d = order;
        w = VectorDouble(n);
        for(int i = 0; i < n; i++) {
            w.at(i) = 0;
            for(int k = std::max(0, i-d); k <= std::min(i, n-d); k++) {
                double p = 1;
                for(int j = k; j < k + d; j++) {
                    if(i != j) {
                        p /= (data.at(i).first-data.at(j).first);
                    }
                }
                if(k % 2 == 1)
                    p = -p;
                w.at(i) += p;
            }
        }
    }
    double operator()(double x) const override {
        double p = 0;
        double q = 0;
        int n = data.size();
        for(int i = 0; i < n; i++) {
            if(x == data[i].first)
                return data[i].second;
            double u = w[i] / (x-data[i].first);
            p += u*data[i].second;
            q += u;
        }
        return p/q;
    }
    std::vector<double> GetWeights() const { return w; }
};

class TrigonometricInterpolator : public AbstractInterpolator {
public:
    TrigonometricInterpolator(const std::vector<std::pair<double, double>> &data): AbstractInterpolator(data) {
        if(std::fabs(data[0].second - data[data.size()-1].second) >= EPSILON) throw std::domain_error("Function is not periodic");
        else this->data.pop_back();
    }
    double operator()(double x) const override {
        double PI = std::atan(1)*4;
        int n = data.size();
        double T = data[n-1].first - data[0].first;
        double omega = 2*PI/T;
        double s = 0;
        if(n % 2 == 0) {
            for(int k = 0; k < n-1; k++) {
                double p = data[k].second;
                for(int j = 0; j < n-1; j++) {
                    if(j == k) continue;
                    p *= (std::sin(omega/2)*(x-data[j].first))/(std::sin(omega/2)*(data[k].first-data[j].first));
                }
                s += p;
            }
        } else {
            for(int k = 0; k < n-1; k++) {
                double p = data[k].second;
                double alpha = 0;
                for(int j = 0; j < n-1; j++) {
                    if(j == k) continue;
                    alpha -= data[j].first;
                    p *= (std::sin(omega/2)*(x-data[j].first))/(std::sin(omega/2)*(data[k].first-data[j].first));
                }
                p *= (std::sin(omega/2)*(x-alpha))/(std::sin(omega/2)*(data[k].first-alpha));
                s += p;
            }
        }
        return s;
    }
};

double P(double x) {
    return 0.5729*x*x*x-2.5253*x*x+5.241*x-2.0002;
}
double trigP(double x) {
    return 0.5*std::sin(2*x) + 2*std::cos(3*x) - std::sin(x);
}

void Testiranje() {
    double donjaGranica = -3, gornjaGranica = 3;
    double testnaDonjaGranica = -3, testnaGornjaGranica = 3;
    double N = 24, gustiN = 50;

    Dataset tackePolinoma;
    Dataset tackeSinusa;
    Dataset gusteTackeSinusa;
    Dataset tackeTrigPolinoma;

    for(double i = donjaGranica; i < gornjaGranica; i+=(gornjaGranica-donjaGranica)/N) {
        tackePolinoma.push_back({i, P(i)});
        tackeSinusa.push_back({i, std::sin(i)});
    }
    for(double i = donjaGranica; i < gornjaGranica; i+=(gornjaGranica-donjaGranica)/gustiN)
        gusteTackeSinusa.push_back({i, std::sin(i)});

    for(double i = -2*PI; i <= 2*PI+0.1; i+=PI/8)
        tackeTrigPolinoma.push_back({i, trigP(i)});

    PolynomialInterpolator pi(tackePolinoma);

    LinearInterpolator li(gusteTackeSinusa);
    PiecewisePolynomialInterpolator ppi(tackeSinusa, 3);
    SplineInterpolator si(tackeSinusa);
    BarycentricInterpolator bi(tackeSinusa, 4);
    TrigonometricInterpolator ti(tackeTrigPolinoma);

    // std::cout << "Tacke polinoma:\n";
    // pi.PrintDataset();
    // std::cout << "\nTacke sinusoide:\n";
    // ppi.PrintDataset();
    // std::cout << "\nTacke guste sinusoide:\n";
    // li.PrintDataset();

    double e1 = 0, e2 = 0, einf = 0;
    std::cout.precision(15);

    std::cout << "Ispitivanje PolynomialInterpolator klase nad polinomom:\n";
    for(double i = testnaDonjaGranica; i <= testnaGornjaGranica; i+=0.25) {
        double greska = pi(i) - P(i);
        e1 += std::fabs(greska);
        e2 += greska*greska;
        einf = std::max(std::fabs(greska), einf);
    }
    std::cout << "E1: " << e1 << ", E2: " << e2 << ", Einf: " << einf << "\n";

    e1 = e2 = einf = 0;

    std::cout << "Ispitivanje LinearInterpolator klase nad sinusom:\n";
    for(double i = testnaDonjaGranica; i <= testnaGornjaGranica; i+=0.25) {
        double greska = li(i) - std::sin(i);
        e1 += std::fabs(greska);
        e2 += greska*greska;
        einf = std::max(std::fabs(greska), einf);
    }
    std::cout << "E1: " << e1 << ", E2: " << e2 << ", Einf: " << einf << "\n";

    e1 = e2 = einf = 0;

    std::cout << "Ispitivanje SplineInterpolator klase nad sinusom:\n";
    for(double i = testnaDonjaGranica; i <= testnaGornjaGranica; i+=0.25) {
        double greska = si(i) - std::sin(i);
        e1 += std::fabs(greska);
        e2 += greska*greska;
        einf = std::max(std::fabs(greska), einf);
    }
    std::cout << "E1: " << e1 << ", E2: " << e2 << ", Einf: " << einf << "\n";

    e1 = e2 = einf = 0;

    std::cout << "Ispitivanje PiecewisePolynomialInterpolator klase nad sinusom:\n";
    for(double i = testnaDonjaGranica; i <= testnaGornjaGranica; i+=0.25) {
        double greska = ppi(i) - std::sin(i);
        e1 += std::fabs(greska);
        e2 += greska*greska;
        einf = std::max(std::fabs(greska), einf);
    }
    std::cout << "E1: " << e1 << ", E2: " << e2 << ", Einf: " << einf << "\n";

    e1 = e2 = einf = 0;

    std::cout << "Ispitivanje BarycentricInterpolator klase nad sinusom:\n";
    for(double i = testnaDonjaGranica; i <= testnaGornjaGranica; i+=0.25) {
        double greska = bi(i) - std::sin(i);
        e1 += std::fabs(greska);
        e2 += greska*greska;
        einf = std::max(std::fabs(greska), einf);
    }
    std::cout << "E1: " << e1 << ", E2: " << e2 << ", Einf: " << einf << "\n";

    e1 = e2 = einf = 0;

    std::cout << "Ispitivanje TrigonometricInterpolator klase nad trigonometrijskim polinomom:\n";
    for(double i = testnaDonjaGranica; i <= testnaGornjaGranica; i+=0.25) {
        double greska = ti(i) - trigP(i);
        e1 += std::fabs(greska);
        e2 += greska*greska;
        einf = std::max(std::fabs(greska), einf);
    }
    std::cout << "E1: " << e1 << ", E2: " << e2 << ", Einf: " << einf << "\n";
}

int main() {
    Testiranje();
    return 0;
}
