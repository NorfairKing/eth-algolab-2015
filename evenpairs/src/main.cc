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
  return (n & 1) ^ 1; // The last bit of n must be 0.
}

// Define s[i] as \sum_{i=0}^n bits[i]
// For each couple i,j the sum of elements between i and j can be found by:
// \sum_{n=i}^j bits[i] = s[j]-s[i]
//
// For evey pair i and j, compute the sum and check whether it's even.
// If so, the ncount it.
int solve(std::vector<int> bits) {
  int n = bits.size();

  std::vector<int> s(n+1);
  s[0] = bits[0];
  for (int i = 1; i <= n; ++i) {
    s[i] = s[i-1] + bits[i-1];
  }

  int even_count = 0;
  for (int i = 0; i <= n; ++i) {
    for (int j = i+1; j <= n; ++j) {
      if (even(s[j] - s[i])) { ++even_count; }
    }
  }

  return even_count;
}
