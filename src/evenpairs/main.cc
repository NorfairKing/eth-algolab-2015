#include <iostream>
#include <vector>

int solve(std::vector<int> bits);

int main() {
  int n;
  std::cin >> n;

  std::vector<int> bits;
  int bit;
  for (int i = 0; i < n; ++i) {
    std::cin >> bit;
    bits.push_back(bit);
  }

  std::cout << solve(bits) << std::endl;
}

int solve(std::vector<int> bits) {
  int even = 1; // The first (empty) sum is even
  int odd = 0;
  int sum = 0;
  int n = bits.size();
  for (int i = 0; i < n; ++i) {
    sum ^= bits[i];
    if (sum == 0) { ++even; }
    else { ++odd; }
  }
  return (even * (even - 1))/2 + (odd * (odd - 1))/2;
}
