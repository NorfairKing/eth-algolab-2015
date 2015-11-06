#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

#include <boost/optional.hpp>

using namespace std;
using namespace boost;

int solve(int n, vector<int>& a, vector<int>& b) {
  optional<int> outcome[n + 1][n + 1]; // i, j

  vector<int> asums(n + 1);
  vector<int> bsums(n + 1);
  asums[n] = bsums[n] = 0;
  for (int x = n; x > 0; x--) {
    asums[x-1] = asums[x] + a[x-1];
    bsums[x-1] = bsums[x] + b[x-1];
  }
  
  outcome[0][0] = 0;
  for (int x = 1; x <= n; ++x) {
    outcome[x][0] = none;
    outcome[0][x] = none;
  }

  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= n; ++j) {
      if (i == 0 && j == 0) { continue; }
      
      optional<int> minimum;
      for (int as = 1; as <= i; ++as) {
        for (int bs = 1; bs <= j; ++bs) {
          optional<int> o = outcome[i - as][j - bs];
          if (!o) { continue; }

          int icost = asums[i-as] - asums[i] - as;
          int jcost = bsums[j-bs] - bsums[j] - bs;
          int cost = icost * jcost;

          int total = o.get() + cost;
  
          if (minimum) {
            minimum = min(minimum.get(), total);
          } else {
            minimum = total;
          }
        }
      }
      outcome[i][j] = minimum;
    }
  }

  return outcome[n][n].get();
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
