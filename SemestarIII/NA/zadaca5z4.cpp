#include <iostream>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <complex>

const double PI = std::atan(1)*4;

std::complex<double> W(double N) { return {std::cos(2*PI/N), std::sin(2*PI/N)}; }

void FFT(std::vector<double> &x, std::vector<std::complex<double>> &xTilde, int N, int s = 0, int d = 0, int t = 1) {
    std::complex<double> WN = W(N);
    if(N == 1)
        xTilde.at(d) = x.at(s);
    else {
        FFT(x, xTilde, N/2, s, d, 2*t);
        FFT(x, xTilde, N/2, s + t, d + N/2, 2*t);
        std::complex<double> mi = 1;
        std::complex<double> w = std::pow(WN, -1);
        for(int k = d; k < d + N/2; k++) {
            std::complex<double> u = xTilde.at(k);
            std::complex<double> v = mi*xTilde.at(k+N/2);
            xTilde.at(k) = u+v;
            xTilde.at(k+N/2) = u-v;
            mi *= w;
        }
    }
}

void InvFFT(std::vector<std::complex<double>> &xTilde, std::vector<std::complex<double>> &x, int N, int s = 0, int d = 0, int t = 1) {
    std::complex<double> WN = W(N);
    if(N == 1)
        x.at(d) = xTilde.at(s);
    else {
        InvFFT(xTilde, x, N/2, s, d, 2*t);
        InvFFT(xTilde, x, N/2, s + t, d + N/2, 2*t);
        std::complex<double> mi = 1;
        std::complex<double> w = WN;
        for(int k = d; k < d + N/2; k++) {
            std::complex<double> u = x.at(k);
            std::complex<double> v = mi*x.at(k+N/2);
            x.at(k) = (u+v)/2.;
            x.at(k+N/2) = (u-v)/2.;
            mi *= w;
        }
    }
}


std::vector<std::complex<double>> toComplexVector(std::vector<double> v) {
    std::vector<std::complex<double>> c;
    for(auto e : v)
        c.push_back(e);
    return c;
}


std::vector<double> LossyCompress(std::vector<double> data, int new_size) {
    if(new_size <= 1 || new_size > data.size()) throw std::range_error("Bad new size");
    int N = data.size();
    if(int(std::log2(N)) != std::log2(N)) throw std::range_error("Data size must be a power of two");

    std::vector<double> y(N);

    for(int n = 0; n <= N/2-1; ++n)
        y.at(n)= data.at(2*n);
    for(int n = N/2; n <= N-1; ++n)
        y.at(n) = data.at(2*(N-n)-1);

    std::vector<std::complex<double>> yTilde(N);
    FFT(y, yTilde, N);
    
    std::vector<double> xTilde(new_size);
    std::complex<double> WN = W(2*N);
    for(int k = 0; k < new_size-1; k++)
        xTilde.at(k) = (std::pow(WN, -k/2.)*yTilde.at(k)).real();

    xTilde.at(new_size-1) = N;
    return xTilde;
}

std::vector<double> LossyDecompress(std::vector<double> compressed) {
    int N = compressed.at(compressed.size()-1);

    if(N < compressed.size()-1 || std::log2(N) != (int)std::log2(N)) throw std::logic_error("Bad compressed sequence");
    if(int(std::log2(N)) != std::log2(N)) throw std::range_error("Data size must be a power of two");

    std::vector<std::complex<double>> yTilde(N);
    yTilde.at(0) = compressed.at(0);

    std::complex<double> WN = W(2*N);
    for(int k = 1; k < compressed.size()-1; k++)
        yTilde.at(k) = 2.*std::pow(WN, k/2.)*compressed.at(k);

    std::vector<std::complex<double>> y(N);
    InvFFT(yTilde, y, N);

    std::vector<double> x(N);
    for(int n = 0; n <= N-1; n++) {
        if(n % 2 == 0)
            x.at(n) = y.at(n/2).real();
        else 
            x.at(n) = y.at(N-(n+1)/2).real();
    }
    return x;
}
 
void Testiranje() {
    std::vector<double> x{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    for(int M = 2; M <= 15; M++) {
        auto xTilde = LossyCompress(x, M);
        auto recovered = LossyDecompress(xTilde);
        double error = 0;
        for(int i = 0; i < x.size(); i++)
            error+=std::fabs(x.at(i)-recovered.at(i));
        std::cout << "Greska za M=" << M << " je " << error << "\n";
    }
}

int main() {
    Testiranje();
    return 0;
}
