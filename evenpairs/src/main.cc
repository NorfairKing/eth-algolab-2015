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
// For a given j:
// - if s[j] is even, s[j]-s[i] is only going to be even is s[i] is also even
// - if s[j] is odd, s[j]-s[i] is only going to be even if s[i] is also odd
//
// Define e[j] as the number of even sums below j
//
// For a given j:
// - if s[j] is even, count e[j] even sums.
// - if s[j] is odd, count j-e[j] even sums.
int solve(std::vector<int> bits) {
  int n = bits.size();

  std::vector<int> s(n+1);
  s[0] = bits[0];
  for (int i = 0; i < n; ++i) {
    s[i+1] = s[i] + bits[i];
  }

  std::vector<int> e(n+1);
  e[0] = 0;
  for (int i = 0; i < n; ++i) {
    e[i+1] = e[i] + even(s[i]);
  }

  int even_count = 0;
  for (int j = 0; j <= n; ++j) {
    if (even(s[j])) {
      even_count += e[j];
    } else {
      even_count += j - e[j];
    }
  }

  return even_count;
}
