#include<stdio.h>
#include<stdlib.h>

int main() {

  int n;
  scanf("%d", &n);
  int* niz = (int*)malloc(n * sizeof(int)); // dinamicki alocirano

  niz[2] = 5;
  for(int i = 0; i < n; i++)
    printf("%d ", niz[i]);
  printf("\n");

  int* niz = (int*)realloc(niz, (n + 2) * sizeof(int));

  for(int i = 0; i < n+2; i++)
    printf("%d ", niz[i]);
  printf("\n");

  free(niz);

  // int* niz2 = (int*)calloc(10, sizeof(int));
  // for(int i = 0; i < 10; i++)
  //   printf("%d ", niz2[i]);
  // printf("\n");

  return 0;
}
