#include<iostream>
#include <unordered_map>

using namespace std;

unordered_map<int, int> cache;

int rob(vector<int>& nums, int from) {
  if(from == nums.size()-1) return nums[from];
  if(from >= nums.size()) return 0;
  int vrijednostAkoUzmem = nums[from] + rob(nums, from+2);
  int vrijednostAkoNeUzmem = rob(nums, from+1);

  if(cache.count(from)) return cache[from];

  return cache[from] = max(vrijednostAkoUzmem, vrijednostAkoNeUzmem);
}
    
int main() {
  vector<int> v{2,7,9,3,1};
  cout << rob(v, 0);
  return 0;
}
