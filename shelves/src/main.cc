#include <iostream>
#include <vector>

struct solution {
  int a;
  int b;
  int e;
};

solution solve(int l, int m, int n);

int main() {
  std::ios_base::sync_with_stdio(false);

  int cases;
  std::cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int l, m, n;
    std::cin >> l;
    std::cin >> m;
    std::cin >> n;

    solution sol = solve(l, m, n);
    std::cout << sol.a << " " << sol.b << " " << sol.e << std::endl;
  }
}

int gcd(int a, int b) {
  if (b == 0) { return a; }
  return gcd(b, a % b);
}

int lcm(int a, int b) {
  return a * b / gcd(a, b);
}

// We're still trying every combination of a and b but inferring b from a.
solution solve(int l, int m, int n) {
  solution bestsol;
  bestsol.a = 0;
  bestsol.b = 0;
  bestsol.e = l;

  solution sol;
  int left;
  for (int a = 0; a <= l / m; ++a) {
    sol.a = a;
    left = l - (sol.a * m);
    sol.b = left / n;
    sol.e = l - (sol.a * m + sol.b * n);
    if (sol.e < bestsol.e) { bestsol = sol; }
    if (sol.e == 0) { break; }
  }
  return bestsol;
}
