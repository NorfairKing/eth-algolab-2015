#include <iostream>
#include <vector>
#include <cmath>


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

#define BASE 10

bool is_int(double n) {
  double intpart;
  return std::modf(n, &intpart) == 0.0;
}

double solve(std::vector<double> nums) {
  int n = nums.size();

  int maxepower = 1;
  for (int i = 0; i < n; ++i) {
    while(!is_int(nums[i] * pow(BASE, maxepower))) { maxepower++; }
  }

  int sum = 0;
  for (int i = 0; i < n; ++i) {
    sum += nums[i] * pow(BASE, maxepower);
  }

  return sum / pow(BASE, maxepower);
}
