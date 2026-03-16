#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

    FILE* file;
    file = fopen("out.dat", "wb");

    int n = 5;
    int* niz = (int*)malloc(n * sizeof(int));

    niz[0] = 0;
    niz[1] = 1;
    niz[2] = 2;
    niz[3] = 3;
    niz[4] = 4;

    fwrite(&n, sizeof(int), 1, file);
    fwrite(niz, sizeof(int), n, file);
    
    int nZaCitanje;
    fread(&nZaCitanje, sizeof(int), 1, file);
    int* nizKojiCitam = (int*)malloc(nZaCitanje*sizeof(int));
    fread(nizKojiCitam, sizeof(int), n, file);

    char str[] = "uup tk vjezbe";

    fwrite(str, sizeof(char), strlen(str), file);

    fclose(file);




}
