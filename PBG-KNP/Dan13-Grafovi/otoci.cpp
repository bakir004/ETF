#include<iostream>
#include<vector>

using namespace std;

class Solution {
    typedef vector<vector<char>> Grid;
public:
    bool visited[300][300];

    bool validno(int i, int j, int m, int n) {
        return i >= 0 && j >= 0 && i < m && j < n;
    }

    void floodfill(Grid& grid, int i, int j) {
        visited[i][j] = true;
        int m = grid.size();
        int n = grid[0].size();
        if(grid[i][j] == '0') return;

        if(validno(i-1, j, m, n) && !visited[i-1][j])
            floodfill(grid, i-1, j);
        if(validno(i+1, j, m, n) && !visited[i+1][j])
            floodfill(grid, i+1, j);
        if(validno(i, j-1, m, n) && !visited[i][j-1])
            floodfill(grid, i, j-1);
        if(validno(i, j+1, m, n) && !visited[i][j+1])
            floodfill(grid, i, j+1);
    }
    int numIslands(Grid& grid) {
        int brojOtoka = 0;
        for(int i = 0; i < grid.size(); i++) {
            for(int j = 0; j < grid[i].size(); j++) {
                if(!visited[i][j] && grid[i][j] == '1') {
                    brojOtoka++;
                    floodfill(grid, i, j);
                }
            }
        }    
        return brojOtoka;
    }
};

int main() {

    return 0;
}
