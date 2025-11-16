#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>

using namespace std;

typedef unordered_map<int, vector<int>> Graf;

int distance(const vector<int>& a, const vector<int>& b) {
  int n = a.size();
  int m = b.size();

  vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

  for (int i = 0; i <= n; ++i) dp[i][0] = i;
  for (int j = 0; j <= m; ++j) dp[0][j] = j;

  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= m; ++j) {
      if (a[i - 1] == b[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1];
      } else {
        dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
      }
    }
  }
  return dp[n][m];
}

void bfs(Graf g, vector<int>& flattened) {
  queue<int> q;
  unordered_map<int,bool> visited;
  q.push(1);
  visited[1] = true;

  while (!q.empty()) {
    int curr = q.front();
    q.pop();
    flattened.push_back(curr);

    sort(g[curr].begin(), g[curr].end());
    for (auto& x : g[curr]) {
      if (!visited[x]) {
        q.push(x);
        visited[x] = true;
      }
    }
  }
}

// Helper to visualize BFS flattened graph
void printFlattened(const vector<int>& flat) {
  cout << "[ ";
  for (auto v : flat) cout << v << " ";
  cout << "]\n";
}

void runTest(Graf g1, Graf g2, const string& testName) {
  vector<int> flat1, flat2;
  bfs(g1, flat1);
  bfs(g2, flat2);

  cout << "=== " << testName << " ===\n";
  cout << "Graph 1 flattened: "; printFlattened(flat1);
  cout << "Graph 2 flattened: "; printFlattened(flat2);
  cout << "Edit distance: " << distance(flat1, flat2) << "\n\n";
}

int main() {
  // Test 1: small modification
  Graf g1;
  g1[1] = {2,3,4};
  g1[2] = {5,6};
  g1[6] = {7,8};

  Graf g2;
  g2[1] = {2,3,4};
  g2[2] = {9};
  g2[9] = {5,6};
  g2[6] = {7,8};

  runTest(g1, g2, "Test 1");

  // Test 2: completely different trees
  Graf g3;
  g3[1] = {2};
  g3[2] = {3};
  g3[3] = {4};

  Graf g4;
  g4[1] = {5};
  g4[5] = {6};
  g4[6] = {7};

  runTest(g3, g4, "Test 2");

  // Test 3: one tree is subset of another
  Graf g5;
  g5[1] = {2,3};
  g5[2] = {4};

  Graf g6;
  g6[1] = {2,3,5};
  g6[2] = {4};
  g6[5] = {6};

  runTest(g5, g6, "Test 3");

  return 0;
}
