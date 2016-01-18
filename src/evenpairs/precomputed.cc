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

inline bool even(int n) {
  return !(n & 1);
}

// Brute force
int solve(std::vector<int> bits) {
  int even_count = 0;

  int n = bits.size();
  int sum[n+1];
  sum[0] = 0;
  for (int i = 0; i < n; ++i) {
    sum[i + 1] = sum[i] + bits[i];
  }
  for (int i = 0; i <= n; ++i) {
    for (int j = i + 1; j <= n; ++j) {
      if (even(sum[j] - sum[i])) { ++even_count; }
    }
  }

  return even_count;
}
