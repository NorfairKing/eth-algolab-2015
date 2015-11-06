#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

#include <boost/optional.hpp>

using namespace std;
using namespace boost;

optional<int> go(vector<int>& a, vector<int>& b, int i, int j) {
  if (i == 0 && j == 0) { return 0; }
  if (i == 0 || j == 0) { return none; }

  int minimum = INT_MAX;
  for (int as = 1; as <= i; ++as) {
    for (int bs = 1; bs <= j; ++bs) {
      optional<int> outcome = go(a, b, i - as, j - bs);
      if (!outcome) { continue; }

      int icost = -as;
      int jcost = -bs;
      for (int x = 0; x < as; x++) { icost += a[i-1-x]; }
      for (int y = 0; y < bs; y++) { jcost += b[j-1-y]; }
      int cost = icost * jcost;

      minimum = min(minimum, outcome.get() + cost);
    }
  }

  return minimum;
}

int solve(int n, vector<int>& a, vector<int>& b) {
  return go(a, b, n, n).get();
}

int main () {
  int cases;
  cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
      cin >> a[i];
    }
    vector<int> b(n);
    for (int i = 0; i < n; ++i) {
      cin >> b[i];
    }
    cout << solve(n, a, b) << endl;
  }
}
