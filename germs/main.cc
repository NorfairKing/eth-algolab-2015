#include <iostream>
#include <vector>

using namespace std;

struct pos {
  int x;
  int y;
};

struct res {
  int f;
  int m;
  int l;
};

int INITIAL_RADIUS = 5;

res solve(int n, int l, int b, int t, vector<pos> ps) {
  res r;
  r.f = 0;
  r.m = 0;
  r.l = 0;
  return r;
}

int main() {
  while (true) {
    int n;
    cin >> n;
    if (n == 0) { break; }
    
    int l, b, r, t;
    cin >> l >> b >> r >> t;
    l *= 10; b *= 10; r *= 10; t *= 10;

    vector<pos> ps;
    for (int i = 0; i < n; ++i) {
      int x, y;
      cin >> x;
      cin >> y;
      x *= 10; y *= 10;
      pos p;
      p.x = x;
      p.y = y;
      ps.push_back(p);
    }

    res sol = solve(n, l, b, t, ps);
    cout << sol.f << " " << sol.m << " " << sol.l << endl;
  }
}
