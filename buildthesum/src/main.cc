#include <iostream>
#include <vector>

double solve(std::vector<double> bits);

int main() {
  int cases;
  std::cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int n;
    std::cin >> n;

    std::vector<double> nums(n);
    for (int i = 0; i < n; ++i) {
      std::cin >> nums[i];
    }

    std::cout << solve(nums) << std::endl;
  }
}

// Just sum all the numbers?
double solve(std::vector<double> nums) {
  int n = nums.size();

  double sum;
  for (int i = 0; i < n; ++i) {
    sum += nums[i];
  }

  return sum;
}
