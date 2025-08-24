#include <iostream>

struct Vektor3d {
  double x, y, z;
};

void UnesiVektor(Vektor3d &v) {
  std::cout << "Unesi x: ";
  std::cin >> v.x;
  std::cout << "Unesi y: ";
  std::cin >> v.y;
  std::cout << "Unesi z: ";
  std::cin >> v.z;
}

Vektor3d ZbirVektora(const Vektor3d &v1, const Vektor3d &v2) {
  return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

double SkalarniProizvod(const Vektor3d &v1, const Vektor3d &v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; 
}

void IspisiVektor(const Vektor3d &v) {
  std::cout << "{" << v.x << "," << v.y << "," << v.z << "}\n";
}

int main() {
  Vektor3d a, b;
  std::cout << "Unesi prvi vektor:\n";
  UnesiVektor(a);
  std::cout << "Unesi drugi vektor:\n";
  UnesiVektor(b);
  std::cout << "Prvi vektor:\n";
  IspisiVektor(a);
  std::cout << "Drugi vektor:\n";
  IspisiVektor(b);
  std::cout << "Suma ova dva vektora je: ";
  IspisiVektor(ZbirVektora(a, b));
  std::cout << std::endl << "Njihov skalarni prozivod je: ";
  std::cout << SkalarniProizvod(a, b);
  return 0;
}
