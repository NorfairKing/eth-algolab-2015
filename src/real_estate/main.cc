#include <iostream>
#include <vector>

using namespace std;

pair<int, int> solve(int n, int m, int s,
                     vector<int>& limits,
                     vector<int>& states,
                     vector<vector<int>>& bids) {

  return make_pair(4, 5);
}

int main() {
  int cases;
  cin >> cases;

  for (int c = 0; c < cases; ++c) {
    int n, m, s;
    cin >> n >> m >> s;

    vector<int> ls(s);
    for (int i = 0; i < s; ++i) {
      cin >> ls[i];
    }

    vector<int> ss(m);
    for (int i = 0; i < m; ++i) {
      cin >> ss[i];
    }

    vector<vector<int>> bs(n, vector<int>(m));
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        cin >> bs[i][j];
      }
    }

    pair<int, int> sol = solve(n, m, s, ls, ss, bs);
    cout << sol.first << " " << sol.second << endl;
  }
}
