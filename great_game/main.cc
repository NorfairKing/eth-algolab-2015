#include <iostream>
#include <vector>
#include <climits>
#include <boost/optional.hpp>

using namespace std;
using namespace boost;

int min_moves(int p, vector<vector<int>>& ts, vector<optional<int>>& mins, vector<optional<int>>& maxs, int n);
int max_moves(int p, vector<vector<int>>& ts, vector<optional<int>>& mins, vector<optional<int>>& maxs, int n);

int max_moves(int p, vector<vector<int>>& ts, vector<optional<int>>& mins, vector<optional<int>>& maxs, int n) {
  if (maxs[p]) { return maxs[p].get(); }

  int cur_max_found = -1;
  int ni = ts[p].size();
  for (int i = 0; i < ni; ++i) { 
    cur_max_found = max(cur_max_found, min_moves(ts[p][i], ts, mins, maxs, n));
  }
  maxs[p] = cur_max_found + 1;
  return maxs[p].get();
}

int min_moves(int p, vector<vector<int>>& ts, vector<optional<int>>& mins, vector<optional<int>>& maxs, int n) {
  if (mins[p]) { return mins[p].get(); }

  int cur_min_found = INT_MAX;
  int ni = ts[p].size();
  for (int i = 0; i < ni; ++i) { 
    cur_min_found = min(cur_min_found, max_moves(ts[p][i], ts, mins, maxs, n));
  }
  mins[p] = cur_min_found + 1;
  return mins[p].get();
}

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

  vector<optional<int>> mins(n);
  vector<optional<int>> maxs(n);

  mins[n - 1] = maxs[n - 1] = 0;
  int r_min = min_moves(r, trans, mins, maxs, n);
  int b_min = min_moves(b, trans, mins, maxs, n);

  int min_sherlock = -1;
  int min_moriarty = -1;

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
