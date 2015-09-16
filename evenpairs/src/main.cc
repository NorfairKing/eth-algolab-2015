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

// Go over every pair i,j; compute the sum of the numbers between i and j
// by adding the jth number for every j and check whether it's even every time.
int solve(std::vector<int> bits) {
  int n = bits.size();

  int even_count = 0;
  for (int i = 0; i < n; ++i) {
    int sum = 0;
    for (int j = i; j < n; ++j) {
      sum += bits[j];
      if (even(sum)) { ++even_count; }
    }
  }

  return even_count;
}
