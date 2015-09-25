#include <iostream>
#include <vector>
#include <cmath>

void solve(long l, long m, long n) {
  long e, a, b;
  e = l;
  a = b = 0;
  
  for (int i = 0; i <= l / m; ++i) {
    int j = (l - i * m) / n;
    int left = l - i * m - j * n;
    if (left < e) {
      e = left;
      a = i;
      b = j;
    }
  }
  std::cout << a << " " << b << " " << e << std::endl;
}

int main() { std::ios_base::sync_with_stdio(false);
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
