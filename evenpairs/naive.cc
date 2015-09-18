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

// Brute force
int solve(std::vector<int> bits) {
  int even_count = 0;

  int n = bits.size();
  for (int i = 0; i < n; ++i) {
    for (int j = i; j < n; ++j) {
      int sum = 0;
      for (int k = i; k <= j; ++k) {
        sum ^= bits[k];
      }
      if (!sum) { ++even_count; }
    }
  }

  return even_count;
}
