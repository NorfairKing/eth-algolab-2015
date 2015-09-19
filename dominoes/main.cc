#include <iostream>
#include <vector>

int solve(std::vector<int> stones);

int main() {
  std::ios_base::sync_with_stdio(false);

  int cases;
  std::cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int n;
    std::cin >> n;

    std::vector<int> stones(n);
    for (int i = 0; i < n; ++i) {
      std::cin >> stones[i];
    }

    std::cout << solve(stones) << std::endl;
  }
}

int solve(std::vector<int> stones) {
  int n = stones.size();
  int last = 0;
  for (int i = 0; i < n; ++i) {
    last = std::max(i + stones[i] - 1, last);
    if (i >= last) { return last + 1; }
  }
  return n;
}
