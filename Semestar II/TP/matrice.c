#include <stdio.h>
double faktorijel(int n) {
    double f = 1;
    for(int i = 1; i <= n; i++) f *= i;
    return f;
}
int main() {
    printf("Tutorijal 6, Zadatak 5");

    int n;
    double x, suma = 0;

    printf("Unesite broj n u intervalu [1, 12]: ");
    scanf("%d", &n);
    printf("Unesite realan broj x: ");
    scanf("%lf", &x);

    for(int i = 1; i <= n; i++)
        suma += x / faktorijel(i);

    printf("Suma: %lf\n", suma);
    return 0;
}
