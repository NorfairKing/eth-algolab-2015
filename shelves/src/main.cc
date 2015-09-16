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

solution solve(int l, int m, int n) {
  solution sol;
  sol.a = 0;
  sol.b = 0;
  sol.e = l;

  // Try as many n's as possible!
  sol.b = l / n;

  // fill up the difference with m's.
  sol.a = (l - sol.b * n) / m;

  solution bestsol = sol;
  int lm = lcm(m, n);
  int olda = sol.a;
  while ((sol.a - olda) * m < lm) {
    sol.e = l - (sol.a * m + sol.b * n);
    if (sol.e == 0) { bestsol = sol; break; }
    if (sol.e >= 0 && sol.e < bestsol.e) { bestsol = sol; }
    if (sol.e < 0) {
      sol.b--;
    } else {
      sol.a++;
    }
  }

  return bestsol;
}
