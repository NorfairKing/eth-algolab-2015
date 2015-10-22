#include <iostream>
#include <vector>

using namespace std;

static vector<vector<int>> s;

int top_color(vector<int>& heights, int col) {
  return s[col][heights[col] - 1];
}

int go(vector<int> heights) {
  int n = heights.size();
  // cout << (1 << (n-1)) << endl;
  // for (int i = 0; i < n; ++i) {
  //   cout << heights[i] << " ";
  // }
  // cout << endl;
  
  int best_score = 0;
  for (int i = 0; i <= (1 << n); ++i) { // N is at most 5 so we can do this.
    // cout << "i: " << i << endl;
    int count = 0;
    bool good_selection = true;
    int color = -1;
    for (int j = 0; j < n; ++j) {
      if ((i >> j) & 1) { // selected
        if (heights[j] <= 0) { 
          good_selection = false;
          break;
        }
        if (color == -1) { // Color hasn't been chosen yet.
          // cout << "getting color" << endl;
          color = top_color(heights, j);
          // cout << "got color" << endl;
        } else {
          // cout << "getting color" << endl;
          if (top_color(heights, j) != color) {
            // cout << "got color" << endl;
            good_selection = false;
            break;
          }
          // cout << "got color" << endl;
        }
        ++count;
      }
    }
    if (count == 0) { continue; }
    if (!good_selection) { continue; }
    vector<int> new_heigths(heights);

    for (int j = 0; j < n; ++j) {
      if ((i >> j) & 1) { // selected
        new_heigths[j]--;
      }
    }

    int this_score;
    if (count == 1) {
      this_score = 0;
    } else {
      this_score = (1 << (count - 2)); // 2 ^ (count - 2)
    }
    // cout << "count: " << count << endl;
    //cout << "this_score: " << this_score << endl;
    int more_score = go(new_heigths);
    int score = this_score + more_score;

    if (score > best_score) {
      best_score = score;
    }
  }
  return best_score;
}

int solve(vector<int>& heights, vector<vector<int>>& stack) {
  s = stack;
  return go(heights);
}

int main() {
  int cases;
  cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int n;
    cin >> n;

    vector<int> heights;
    for (int i = 0; i < n; ++i) {
      int mi;
      cin >> mi;
      heights.push_back(mi);
    }

    vector<vector<int>> stacks(n);
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        int cij;
        cin >> cij;
        stacks[i].push_back(cij);
      }
    }

    cout << solve(heights, stacks) << endl;
    break;
  }
}
