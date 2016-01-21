#include <iostream>
#include <vector>

using namespace std;

struct cell {
  int x;
  int y;
  int z;
};

int solve(int h, int t, vector<cell>& healthy, vector<cell>& tumor) {
  return 5;
}

int main() {
  int cases;
  cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int h, t;
    cin >> h >> t;

    vector<cell> hs(h);
    for (int i = 0; i < h; ++i) {
      cin >> hs[i].x;
      cin >> hs[i].y;
      cin >> hs[i].z;
    }
    vector<cell> ts(h);
    for (int i = 0; i < t; ++i) {
      cin >> ts[i].x;
      cin >> ts[i].y;
      cin >> ts[i].z;
    }

    int sol = solve(h, t, hs, ts);
    if (sol > 30) {
      cout << "Impossible!";
    } {
      cout << sol;
    }
    cout << endl;
  }
}
