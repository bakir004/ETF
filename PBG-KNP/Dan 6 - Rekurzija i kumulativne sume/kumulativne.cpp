#include<iostream>

using namespace std;

int main() {
  vector<int> nums{5,7,8,9,1,3,7};
  vector<int> sume(nums.size());

  sume[0] = nums[0];
  for(int i = 1; i < nums.size(); i++) {
    sume[i] = sume[i-1] + nums[i];
  }

  for(int i = 0; i < nums.size(); i++) 
    cout << i << " ";
  cout << "\n";
  for(int i = 0; i < nums.size(); i++) 
    cout << nums[i] << " ";
  cout << "\n";
  while(true) {
    int i,j;
    cin >> i >> j;
    cout << "suma: " << sume[j] - sume[i-1] << "\n";
  }
  return 0;
}
