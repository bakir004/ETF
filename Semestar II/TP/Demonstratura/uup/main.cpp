#include <stdio.h>

int main()
{
  int n, niz[100];
  do
  {
    printf("Unesi broj n:");
    scanf("%d", &n);
  } while (n < 1 || n > 100);
  int i = 1, j, m;
  j = n;
  for (m = 0; m < n; m++)
  {
    if (m % 2 == 0)
    {
      niz[m] = i;
      i++;
    }
    else
    {
      niz[m] = j;
      j--;
    }
  }
  for (i = 0; i < n; i++)
  {
    printf("%d ", niz[i]);
  }
  return 0;
}