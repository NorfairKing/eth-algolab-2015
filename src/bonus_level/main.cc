#include <iostream>
#include <vector>

using namespace std;

int inBounds(int n, int x, int y) {
  return x >= 0 && y >= 0 && x < n && y < n;
}

// Insight 1: going down and back up again is equivalent to going down twice because of how we are restricted in movement.
// Insight 2: We can use dynamic programming IF we pretend to run the two journeys in parallel.
//
// In each step, mario will be on the same diagonal in both travels.
//
// We keep a dynamic programming state:
//
// state[][][]
//
// The first dimension represents the diagonal of the step: x + y.
// The second dimension represents the x-position of the first travel.
// The third dimension represents the x-position of the second travel.
//
// The value in the array is the total number of coins that can be collected when starting from that point.
//
// Then we will work backwards until we fill [0][0][0]. That will be the result.
int solve(int n, vector<vector<int>>& shelves) {
  // DP state
  vector<vector<vector<int >>>     state(2 * n - 1, vector<vector<int >>(n    , vector<int >(n    , 0)));
  // Lookup table for what should be a local function.
  vector<vector<vector<bool>>> in_bounds(2 * n    , vector<vector<bool>>(n + 1, vector<bool>(n + 2, false)));

  for (int d = 2 * n - 2; d >= 0; --d) {
    for (int x1 = 0; x1 < n; ++x1) {
      for (int x2 = 0; x2 < n; ++x2) {
        state[d][x1][x2] = 0;

        int y1 = d - x1;
        int y2 = d - x2;
        in_bounds[d][x1][x2] = y1 >= 0 && y1 < n
                            && y2 >= 0 && y2 < n;
      }
    }
  }

  for (int d = 2 * n - 2; d >= 0; --d) {
    for (int x1 = 0; x1 < n; ++x1) {
      int y1 = d - x1;

      // Check for valid bounds
      if (! (y1 >= 0 && y1 < n)) { continue; }

      for (int x2 = 0; x2 < n; ++x2) {
        int y2 = d - x2;

        // Check for valid bounds
        if (! (y2 >= 0 && y2 < n)) { continue; }

        // We are now at (x1, y1) on the first travel and at (x2, y2) on the second travel.
        // The In the first travel we can go up or left and the same thing hold for the second travel.
        // This means that we have 4 possible previous states to consider:
        // - [d + 1][x1    ][x2    ]
        // - [d + 1][x1 + 1][x2    ]
        // - [d + 1][x1    ][x2 + 1]
        // - [d + 1][x1 + 1][x2 + 1]
        //
        // In any case, p1 picks up (x1, y1) and p2 picks up (x2, y2)

        int m = 0;
        if (in_bounds[d + 1][x1    ][x2    ]) { m = max(m, state[d + 1][x1    ][x2    ]); }
        if (in_bounds[d + 1][x1 + 1][x2    ]) { m = max(m, state[d + 1][x1 + 1][x2    ]); }
        if (in_bounds[d + 1][x1    ][x2 + 1]) { m = max(m, state[d + 1][x1    ][x2 + 1]); }
        if (in_bounds[d + 1][x1 + 1][x2 + 1]) { m = max(m, state[d + 1][x1 + 1][x2 + 1]); }

        state[d][x1][x2] = shelves[x1][y1] + shelves[x2][y2] + m;
        if (x1 == x2 && y1 == y2) { state[d][x1][x2] -= shelves[x1][y1]; } // Don't count doubles
      }
    }
  }

  return state[0][0][0];
}

int main() {
  ios_base::sync_with_stdio(false);
  int cases;
  cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int n;
    cin >> n;

    vector<vector<int>> cs(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        cin >> cs[i][j];
      }
    }

    cout << solve(n, cs) << endl;
  }
}
