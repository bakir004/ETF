vector<int> twoSum(vector<int>& nums, int target) {
    sort(nums.begin(), nums.end());

    int l = 0, r = nums.size()-1;
    while(l < r) {
        int suma = nums[l] + nums[r];
        if(suma == target) return {l,r};
        if(suma < target) l++;
        else if(suma > target) r--;
    }
    return {0,0};
}
