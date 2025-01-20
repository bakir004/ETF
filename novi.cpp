#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

const double pi = 4 * atan(1);

void FFT(std::vector<double> &x, std::vector<std::complex<double>> &y, int n,
         int s = 0, int d = 0, int t = 1) {

  if (n == 1)
    y[d] = x[s];

  else {
    FFT(x, y, n / 2, s, d, t * 2);
    FFT(x, y, n / 2, s + t, d + n / 2, t * 2);
    std::complex<double> mi = 1;
    std::complex<double> w = std::pow(
        std::complex<double>(std::cos(2 * pi / n), std::sin(2 * pi / n)), -1);

    for (int k = d; k < d + n / 2; k++) {
      std::complex<double> c = y[k];
      std::complex<double> c2 = mi * y[k + n / 2];
      y[k] = c + c2;
      y[k + n / 2] = c - c2;
      mi *= w;
    }
  }
}

void invFFT(std::vector<std::complex<double>> &y,
            std::vector<std::complex<double>> &x, int n, int s = 0, int d = 0,
            int t = 1) {

  if (n == 1)
    x[d] = y[s];

  else {
    invFFT(y, x, n / 2, s, d, t * 2);
    invFFT(y, x, n / 2, s + t, d + n / 2, t * 2);
    std::complex<double> mi = 1;
    std::complex<double> w =
        std::complex<double>(std::cos(2 * pi / n), std::sin(2 * pi / n));

    for (int k = d; k < d + n / 2; k++) {
      std::complex<double> c = x[k];
      std::complex<double> c2 = mi * x[k + n / 2];
      x[k] = (c + c2) / 2.0;
      x[k + n / 2] = (c - c2) / 2.0;
      mi *= w;
    }
  }
}

std::vector<double> LossyCompress(std::vector<double> data, int new_size) {

  if (new_size < 2 || new_size > data.size())
    throw std::range_error("Bad new size");

  if (!((data.size() & (data.size() - 1)) == 0))
    throw std::range_error("Data size must be a power of two");

  int n = data.size();
  std::vector<double> v1(n);
  std::vector<std::complex<double>> v2(n);

  for (int i = 0; i <= n / 2 - 1; i++)
    v1[i] = data[i * 2];
  for (int i = n / 2; i <= n - 1; i++)
    v1[i] = data[2 * (n - i) - 1];

  FFT(v1, v2, n);

  std::vector<double> sekvenca(new_size);
  for (int i = 0; i < new_size - 1; i++) {
    std::complex<double> w =
        std::complex<double>(std::cos(pi / n), std::sin(pi / n));
    w = std::pow(w, (-1 * i) / 2.);
    sekvenca[i] = (w * v2[i]).real();
  }
  sekvenca[new_size - 1] = n;
  return sekvenca;
}

std::vector<double> LossyDecompress(std::vector<double> compressed) {

  int n = compressed[compressed.size() - 1];

  if (n < 1 || n < compressed.size())
    throw std::logic_error("Bad compressed sequence");

  if (!((n & n - 1) == 0))
    throw std::range_error("Data size must be a power of two");

  std::vector<std::complex<double>> y(n);
  std::vector<std::complex<double>> x(n);
  y[0] = compressed[0];

  for (int k = 1; k < compressed.size() - 1; k++) {
    std::complex<double> w =
        std::complex<double>(std::cos(pi / n), std::sin(pi / n));
    w = std::pow(w, k / 2.);
    y[k] = 2.0 * w * compressed[k];
  }

  invFFT(y, x, n);

  std::vector<double> recon(n);
  for (int i = 0; i <= n - 1; i++) {
    if (i % 2 == 0)
      recon[i] = x[i / 2].real();
    else
      recon[i] = x[n - (i + 1) / 2].real();
  }

  return recon;
}

int main() {
  std::vector<double> start = {1, 2,  3,  4,  5,  6,  7,  8,
                               9, 10, 11, 12, 13, 14, 15, 16};

  auto compressed = LossyCompress(start, 5);
  for (const auto &x : compressed)
    std::cout << x << " ";

  std::cout << std::endl;

  auto end = LossyDecompress(compressed);
  for (const auto &x : end)
    std::cout << std::round(x) << " ";
  return 0;
}
