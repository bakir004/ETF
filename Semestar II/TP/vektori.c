#include <stdio.h>
#include <math.h>

#define EPS 1e-9

struct Vektor {
    double x, y, z;
};

double dotProduct(struct Vektor a, struct Vektor b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Vektor crossProduct(struct Vektor a, struct Vektor b) {
    struct Vektor result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

int main() {
    struct Vektor niz[] = {
        {1.0, 0.0, 0.0}, 
        {0.0, 1.0, 0.0},
        {1.0, 1.0, 0.0}, 
        {-1.0, 1.0, 0.0},
    };

    int n = sizeof(niz) / sizeof(niz[0]);

    printf("Perpendicular vector pairs (dot product ≈ 0):\n\n");

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double dot = dotProduct(niz[i], niz[j]);

            if (fabs(dot) < EPS) {
                printf(
                    "V%d (%.2f, %.2f, %.2f) ⟂ V%d (%.2f, %.2f, %.2f)\n",
                    i, niz[i].x, niz[i].y, niz[i].z,
                    j, niz[j].x, niz[j].y, niz[j].z
                );

                struct Vektor cp = crossProduct(niz[i], niz[j]);
                printf(
                    "  Cross product: (%.2f, %.2f, %.2f)\n\n",
                    cp.x, cp.y, cp.z
                );
            }
        }
    }

    return 0;
}
