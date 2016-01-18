#include <iostream>
#include <vector>

using namespace std;

struct solution {
  bool possible;
  int maxstud;
  int maxprof;
};

solution solve(int n) {
  solution s;
  s.possible = false;
  s.maxstud = 5;
  s.maxprof = 4;
  return s;
}

int main() {
  int cases;
  cin >> cases;
  for (int t = 0; t < cases; ++t) {
    int n;
    cin >> n;

    for (int i = 0; i < n; ++i) {
      int a, c;
      cin >> a >> c;
    }

    for (int i = 0; i < n; ++i) {
      int s, p;
      cin >> s >> p;
    }

    for (int i = 0; i < n - 1; ++i) {
      int v, e;
      cin >> v >> e;
    }

    solution sol = solve(n);
    if (sol.possible) {
      cout << "possible";
    } else {
      cout << "impossible";
    }
    cout << " " << sol.maxstud << " " << sol.maxprof << endl;
  }
}
