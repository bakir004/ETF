#include <stdio.h>

int main() {
  int n;
  scanf("%d", &n);

  int a = 'A';
  for(int i = 0; i < n; i++) {
    char trenutni = a;
    for(int j = 0; j < n; j++) {
      if(i == 0 || i == n-1) {
        printf("%c ", a+j);
        continue;
      }
      if(i == j) {
        printf("%c ", trenutni);
        trenutni++;
      } else if(i + j == n-1) {
        printf("%c ", trenutni);
        trenutni++;
      } else if(i + j > n-1)  {
        printf("  ");
        trenutni++;
      } else if(j > i) {
        printf("  ");
        trenutni++;
      } else if(j < i) printf("  ");
    }
    printf("\n");
  }
}
