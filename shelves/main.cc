#include <iostream>
#include <vector>
#include <cmath>


void part1(long l, long m, long n) {
  long e, a, b;
  e = l;
  a = 0;
  b = 0;

  for (int j = l / n; j >= 0; --j) {
      int i = (l - j * n) / m;
      int left = l - i * m - j * n;
      if (left < e) {
        e = left;
        a = i;
        b = j;
      }
  }
  std::cout << a << " " << b << " " << e << std::endl;
}

void part2(long l, long m, long n) {
  long e, a, b;
  e = l;
  a = 0;
  b = 0;
  
  for (int i = 0; i < l / m && i <= n; ++i) {
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

void solve(long l, long m, long n){
  if (n * n > l) {
    part1(l, m, n);
  } else {
    part2(l, m, n);
  }
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
