#include <stdio.h>
#include <string.h>

struct Timestamp {
    int godina, mjesec, dan, sat, minuta;
};

int Prije(struct Timestamp t1, struct Timestamp t2) {
    if(t1.godina != t2.godina) return t1.godina < t2.godina;
    if(t1.mjesec != t2.mjesec) return t1.mjesec < t2.mjesec;
    if(t1.dan != t2.dan) return t1.dan < t2.dan;
    if(t1.sat != t2.sat) return t1.sat < t2.sat;
    if(t1.minuta != t2.minuta) return t1.minuta < t2.minuta;
    return 0;
}

int main() {
    char nesta[1000];
    char str[100];
    sprintf(str, "%d", 999999);
    // printf("%s", str);
    
    for(int i = 0; i < strlen(str); i++)
        nesta[i] = str[i];

    printf("%s", nesta);

}










