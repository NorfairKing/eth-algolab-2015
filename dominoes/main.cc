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

// Keep a counter of the last domino that's definitely going to fall
// Go over all the dominoes until that counter and make them topple,
// that means updating the counter to reflect the new last domino.
int solve(std::vector<int> stones) {
  int last = 0;
  int i = 0;
  do { last = std::max(i + stones[i] - 1, last); } while (i++ < last);
  return last + 1;
}
