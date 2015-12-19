#include <iostream>
#include <vector>
#include <boost/optional.hpp>

using namespace std;
using namespace boost;

bool even(int n) {
  return n % 2 == 0;
}

// Even turns are mine.
int at_least(int turn, vector<int>& values, int i, int j, vector<vector<optional<int>>>& state) {
  if (state[i][j]) {
    return state[i][j].get();
  } else {
    if (i > j) { return 0; }
    bool e = even(turn);
    if (i == j) {
      if (e) {
        return values[i];
      } else {
        return 0;
      }
    }

    int nt = turn + 1;
    int a1 = at_least(nt, values, i + 1, j, state);
    int a2 = at_least(nt, values, i, j - 1, state);
    int val;
    if (e) {
      val = max(values[i] + a1, values[j] + a2);
    } else {
      val = min(a1, a2);
    }
    state[i][j] = val;
    return val;
  }
}

int solve(int n, vector<int>& values) {
  vector<vector<optional<int>>> state(n, vector<optional<int>>(n));
  return at_least(0, values, 0, n - 1, state);
}

int main() {
  int cases;
  cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int n;
    cin >> n;

    vector<int> vs(n);
    for (int i = 0; i < n; ++i) {
      cin >> vs[i];
    }

    cout << solve(n, vs) << endl;
  }
}
