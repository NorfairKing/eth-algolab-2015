#include <iostream>
#include <vector>
#include <cmath>

void solve(long l, long m, long n);

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
  long bestE, bestA, bestB;
  bestE = l;
  int e;
  for (int a = 0; a <= l / m; ++a) {
    for (int b = 0; b <= l / n; ++b) {
      e = l - (a * m + b * n);
      if (e >= 0 && e < bestE) {
        bestE = e;
        bestA = a;
        bestB = b;
      }
    }
  }
  std::cout << bestA << " " << bestB << " " << bestE << std::endl;
}
