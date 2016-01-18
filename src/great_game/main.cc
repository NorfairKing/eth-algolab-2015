#include <iostream>
#include <vector>
#include <climits>
#include <boost/optional.hpp>

using namespace std;
using namespace boost;

struct transition {
  int from;
  int to;
};

bool odd(int n) {
  return (n & 1);
}

bool solve(int n, int m, int r, int b, vector<transition>& ts) {
  vector<vector<int>> trans(n);
  for (int i = 0; i < m; ++i) {
    trans[ts[i].from].push_back(ts[i].to);
  }

  vector<int> mins(n, 0);
  vector<int> maxs(n, 0);

  for (int i = n - 2
      ; i >= min(r, b) // No need to go any further
      ; --i) {
    mins[i] = INT_MAX;
    maxs[i] = 0;

    int deg = trans[i].size();
    for (int j = 0; j < deg; ++j) {
      int k = trans[i][j];
      mins[i] = min(mins[i], 1 + maxs[k]);
      maxs[i] = max(maxs[i], 1 + mins[k]);
    }
  }

  int r_min = mins[r];
  int b_min = mins[b];

  int min_sherlock;
  int min_moriarty;

  // S Red
  // M Black
  // M Black
  // S red
  //
  // Red and black each get one move every two
  if(odd(r_min)) {
    min_sherlock = (r_min - 1) * 2 + 0;
  } else {
    min_sherlock = (r_min - 2) * 2 + 3;
  }

  if(odd(b_min)) {
    min_moriarty = (b_min - 1) * 2 + 1;
  } else {
    min_moriarty = (b_min - 2) * 2 + 2;
  }

  return min_sherlock > min_moriarty;
}

int main() {
  int cases;
  cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int n, m;
    cin >> n >> m;
    int r, b;
    cin >> r >> b;
    --r; --b; // Zero indexed

    vector<transition> ts;
    for (int i = 0; i < m; ++i) {
      int u, v;
      cin >> u >> v;
      transition t;
      t.from = u - 1; // zero indexed
      t.to = v - 1; // zero indexed
      ts.push_back(t);
    }
    cout << solve(n, m, r, b, ts) << endl;
  }
}
