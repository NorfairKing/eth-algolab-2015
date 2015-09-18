#include <iostream>
#include <vector>
#include <cmath>

void solve(long l, long m, long n);

// Try the O(l/n) algorithm if n is larger than sqrt(l), otherwise try the O(n) algorithm.
// O(sqrt(l))
int main() {
  std::ios_base::sync_with_stdio(false);

  int cases;
  std::cin >> cases;
  for (int c = 0; c < cases; ++c) {
    long l, m, n;
    std::cin >> l;
    std::cin >> m;
    std::cin >> n;

    solve(l, m, n);
  }
}

void solve(long l, long m, long n) {
  long bestA, bestB, bestE;
  bestA = 0;
  bestB = 0;
  bestE = l;
  for (int b = 0; b <= l / n; ++b) {
    long left = l - b * n;
    long a = left / m;
    long e = l - (a * m + b * n);
    if (e <= bestE) {
      bestE = e;
      bestA = a;
      bestB = b;
    }
  }
  std::cout << bestA << " " << bestB << " " << bestE << std::endl;
}
