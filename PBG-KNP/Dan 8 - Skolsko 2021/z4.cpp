#include<iostream>
#include<vector>

using namespace std;

bool vanGranica(int i, int j, int m, int n) {
  if(i < 0 || j < 0 || i >= m || j >= n) return true;
  return false;
}

int main() {
  int m,n;
  cin >> m >> n;
  vector<vector<int>> ploca(m, vector<int>(n, 0));

  for(int i = 0; i < m; i++)
    for(int j = 0; j < n; j++)
      cin >> ploca[i][j];

  int ukupnaPovrsina = 2*m*n;

  for(int i = 0; i < m; i++) {
    for(int j = 0; j < n; j++) {
      if(vanGranica(i-1, j, m, n)) ukupnaPovrsina += ploca[i][j];
      else if(ploca[i-1][j] < ploca[i][j]) ukupnaPovrsina += ploca[i][j]-ploca[i-1][j];
      if(vanGranica(i+1, j, m, n)) ukupnaPovrsina += ploca[i][j];
      else if(ploca[i+1][j] < ploca[i][j]) ukupnaPovrsina += ploca[i][j]-ploca[i+1][j];
      if(vanGranica(i, j-1, m, n)) ukupnaPovrsina += ploca[i][j];
      else if(ploca[i][j-1] < ploca[i][j]) ukupnaPovrsina += ploca[i][j]-ploca[i][j-1];
      if(vanGranica(i, j+1, m, n)) ukupnaPovrsina += ploca[i][j];
      else if(ploca[i][j+1] < ploca[i][j]) ukupnaPovrsina += ploca[i][j]-ploca[i][j+1];
    }
  }

  cout << ukupnaPovrsina;

  return 0;
}
