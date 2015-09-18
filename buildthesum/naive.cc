#include <iostream>
#include <vector>
#include <cmath>

#define BASE 10

double solve(std::vector<double> nums);

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

bool is_int(double n) {
  double intpart;
  return std::modf(n, &intpart) == 0.0;
}

// Bring numbers into an integer representation first, sum them and then bring
// them back to a double.
double solve(std::vector<double> nums) {
  int n = nums.size();
  double sum = 0;
  for (int i = 0; i < n; ++i) {
    sum += nums[i];
  }
  return sum;
}
