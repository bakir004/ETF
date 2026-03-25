#include<iostream>

using namespace std;

int main() {
  vector<int> nums{5,7,8,-9,-1,-4,-1};
  vector<int> sume(nums.size());

  sume[0] = nums[0];
  for(int i = 1; i < nums.size(); i++) {
    sume[i] = sume[i-1] + nums[i];
  }

  if(sume[nums.size()-1] - sume[0] == 0) 
    cout << "pivot index je 0\n";
  for(int i = 1; i < nums.size(); i++) {
    if(sume[i-1] == sume[nums.size()-1] - sume[i])
      cout << "pivot index je " << i << "\n";
  }
  return 0;
}
