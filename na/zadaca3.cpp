#include <iostream>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <algorithm>

typedef std::pair<double, double> Par;
typedef std::vector<Par> Dataset;
typedef std::vector<double> VectorDouble;

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
        for(int i = 0; i < data.size(); i++)
            std::cout << "" << data[i].first << " " << data[i].second << "\n";
        std::cout << "\n";
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
        r[0] = 0;
        r[n-1] = 0;
        VectorDouble alpha(n);
        for(int i = 1; i <= n-2; i++) {
            alpha[i] = 2*(data.at(i+1).first-data.at(i-1).first);
            r[i] = 3*((data.at(i+1).second-data.at(i).second)/(data.at(i+1).first-data.at(i).first)-
                      (data.at(i).second-data.at(i-1).second)/(data.at(i).first-data.at(i-1).first));
        }
        for(int i = 1; i <= n-2; i++) {
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

        /*std::cout << "x: " << x << ", i: " << i << ", found: " << foundInterval <<  "\n";*/

        /*double xn = data.at(n-1).first, yn = data.at(n-1).second;*/
        /*double xi = data.at(i).first, yi = data.at(i).second;*/
        /*double t = x-xi;*/
        /*double deltaX = data.at(i+1).first - xi;*/
        /*double s = (r.at(i+1)-r.at(i))/(3*deltaX);*/
        /*double q = (data.at(i+1).second-yi)/deltaX - deltaX*(r.at(i+1)+2*r.at(i))/3;*/


        /*return data.at(i).second + t*(q+t*(r.at(i)+t*s));*/
        double t = x-data[i].first;
        return data[i].second + t *(q[i] + t*(r[i] + t*s[i]));
    }
};

/*if(foundInterval == n) {*/
/*std::cout << q << "(x-" << xi << ")+" << r[i] << "(x-" << xi << ")^2+" << s << "(x-" << xi << ")^3\n";*/
/*    double k = q*/
/*        + 2*r.at(i)*(xn-xi)*/
/*        + 3*s*(xn-xi)*(xn-xi);*/
/*    double m = yn-k*xn;*/
/*    std::cout << "k: " << k << ", m: " << m << "\n";*/
/*    return k*x + m;*/
/*} else if(foundInterval == 0) {*/
/*    double k = q */
/*        + 2*r.at(i)*(data.at(0).first-data.at(i).first)*/
/*        + 3*s*(data.at(0).first-data.at(i).first)*(data.at(0).first-data.at(i).first);*/
/*    double m = data.at(0).second-k*data.at(0).first;*/
/*    std::cout << "k: " << k << ", m: " << m << "\n";*/
/*    return k*x + m;*/
/*}*/

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

int main() {
    const double PI3=std::atan(1)*4;
    std::vector<std::pair<double,double>> data3;
    for(double i=0; i<=2*PI3; i+=PI3/2)
        data3.push_back({i,std::cos(i)});
    SplineInterpolator si3(data3);
    si3.PrintDataset();
    std::cout<<si3(-0.1)<<" "<<std::cos(-0.1)<<std::endl;
    std::cout<<si3(-0.2)<<" "<<std::cos(-0.2)<<std::endl;
    std::cout<<std::round(si3(PI3/2))<<" "<<std::round(std::cos(PI3/2))<<std::endl;
    std::cout<<si3(PI3/2+0.1)<<" "<<std::cos(PI3/2+0.1)<<std::endl;
    std::cout<<si3(PI3*3+0.1)<<" "<<std::cos(PI3*3+0.1)<<std::endl;
    std::cout<<si3(PI3*3+0.2)<<" "<<std::cos(PI3*3+0.2);
    return 0;
}
