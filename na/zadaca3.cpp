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
        if(data[lastAccessedInterval-1].first < x && x <= data[lastAccessedInterval-1].first) 
            return lastAccessedInterval;
        // treba susjedne provjerit
        if(x > data[data.size()-1].first) return data.size();
        if(x < data[0].first) return 0;
        auto p = std::lower_bound(data.begin(), data.end(), Par(x,0), [](Par p1, Par p2){
            return p1.first < p2.first;
        });
        return lastAccessedInterval = p-data.begin();
    }
public:
    AbstractInterpolator(const Dataset &data) {
        lastAccessedInterval = 1;
        this->data = data;
        std::sort(this->data.begin(), this->data.end(), [](Par p1, Par p2){
            if(p1.first == p2.first) 
                throw std::domain_error("Invalid data set");
            return p1.first < p2.first;
        });
    }
    virtual double operator()(double x) const = 0;
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
                v[j] = w[j];
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
        this->k = order;
    }
    double operator()(double x) const override {
        int foundInterval = Locate(x);
        int n = data.size();
        int i = clamp(k/2+1, n-(k/2+1), foundInterval)-1;
        Dataset subset;
        if(k % 2 == 1) {
            for(int j = i-(k-1)/2; j <= i+(k+1)/2; j++)
                subset.push_back(data[j]);
        } else {
            for(int j = i-k/2; j <= i+k/2; j++)
                subset.push_back(data[j]);
        }
        for(int i = 0; i < subset.size(); i++)
            std::cout << "(" << subset[i].first << "," << subset[i].second << ")\n";
        std::cout << "\n";
        double s = 0;
        for(int i = 0; i < n; i++) {
            double p = data[i].second;
            for(int j = 0; j < n; j++)
                if(j != i)
                    p *= (x-data[j].first)/(data[i].first-data[j].first);
            s += p;
        }
        return s;
    }
};

int main() {
    PiecewisePolynomialInterpolator pwi({{0, 8}, {1, 3}, {2, 7}, {4, 5}, {6, 8},
            {8, 7}, {10, 5}}, 3);
    for(double x : {0.5, 1.5, 3., 5., 7., 9.}) std::cout << pwi(x) << " ";
    return 0;
}
