#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <set>
#include <boost/optional.hpp>

using namespace std;
using namespace boost;

int solve(vector<vector<int>>& positions) {
  int n = positions.size();

  priority_queue<pair<int, int>, vector<pair<int, int>>> poss; // position, word
  for (int i = 0; i < n; ++i) {
    int m = positions[i].size();
    for (int j = 0; j < m; ++j) {
      poss.push(make_pair(positions[i][j], i));
    }
  }

  int min_dist = INT_MAX;
  
  // Keep track of the last chosen position for each word
  vector<int> chosen_position(n, -1);
  // Keep track of the current set of chosen words
  set<pair<int, int>> chosen; // position, word
  int found_words = 0;
  while (!poss.empty()) {
    pair<int, int> tuple = poss.top();
    poss.pop();

    int pos = tuple.first;
    int word = tuple.second;

    if (chosen_position[word] == -1) { // not yet but found just now
      ++found_words;
    } else {
      chosen.erase(make_pair(chosen_position[word], word)); // erase the old tuple
    }

    chosen_position[word] = pos; // Set new chosen position
    chosen.insert(tuple);

    if (found_words == n) {
      int b = (--chosen.end())->first; // For some reason end is weird?
      int a = chosen.begin()->first;
      int dist = b - a;
      if (dist < min_dist) { min_dist = dist; }
    }
  }

  return min_dist + 1;
}

int main() {
  ios_base::sync_with_stdio(false);

  int cases;
  cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int n;
    cin >> n;
    vector<int> occs(n);
    for (int w = 0; w < n; ++w) {
      cin >> occs[w];
    }
    vector<vector<int>> poss(n);
    for (int i = 0; i < n; ++i) {
      poss[i] = vector<int>(occs[i]);
      for (int j = 0; j < occs[i]; ++j) {
        cin >> poss[i][j];
      }
    }
    cout << solve(poss) << endl;
  }
}
