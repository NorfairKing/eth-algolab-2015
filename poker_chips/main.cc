#include <iostream>
#include <vector>

#include <boost/optional.hpp>

using namespace std;
using namespace boost;

static vector<int> s;
static vector<vector<int>> st;
static vector<int> h;
static int n;

int index(vector<int>& acc) {
  int ss = 1;
  int ix = 0;
  for (int i = 0; i < n; ++i) {
    ix += ss * acc[i];
    ss *= (h[i] + 1);
  }
  return ix;
}

vector<int> accessor(int ix) {
  vector<int> acc(n);
  for (int i = 0; i < n; ++i) {
    acc[i] = ix % (h[i] + 1); // Same comment, n+1 states.
    ix /= (h[i] + 1);
  }
  return acc;
}

int set(long val, int bit) {
  return (val >> bit) & 1;
}

int bitsset(long val) {
  int numset = 0;
  for (int c = 0; c < n; ++c) { // c is a chip
    if (set(val, c)) {
      ++numset;
    }
  }
  return numset;
}

int solve(vector<int>& heights, vector<vector<int>>& stack) {
  h = heights;
  st = stack;
  n = h.size();


  int prod = 1;
  for (int i = 0; i < n; ++i) {
    prod *= h[i] + 1; // One more because a stack with n chips has n + 1 states. (including empty and full)
  }

  vector<int> ss(prod, 0);
  s = ss;

  long pn = 1 << n; // Number of subsets

  vector<int> acc(n);
  for (int ix = 0; ix < prod; ++ix) {
    acc = accessor(ix);

    for (long x = 0; x < pn; ++x) { // x is a subset
      // If bit i is set, then it's chosen
      
      // If none of the bits are chosen, don't do anything.
      int numset = bitsset(x); // Number of bits set / stacks chosen
      if (numset == 0) { continue; }

      // Construct the accessor for the situation in which you take away the chosen chips
      vector<int> one_less_than_acc(acc);
      for (int c = 0; c < n; ++c) {
        if (set(x, c)) {
          --one_less_than_acc[c];
        }
      }

      // Check if it's valid
      bool good = true;
      for (int c = 0; c < n; ++c) {
        if (one_less_than_acc[c] < 0) {
          good = false;
          break;
        }
      }
      if (! good) { continue; } // If one_less contains a negative number then we've chosen from an empty stack.

      // Check if the chosen chips have the same color.
      bool all_same = true;
      optional<int> col;
      for (int c = 0; c < n; ++c) { // c is a chip
        if (!set(x, c)) { continue; }

        int thiscol = stack[c][acc[c] - 1]; // acc[c] is the height of stack c in this situation
        if (!col) {
          col = thiscol;
          continue;
        }
        if (thiscol != col.get()) {
          all_same = false;
          break;
        }
      }
      if (!all_same) { continue; } // if they're not all the same color then this is not a good choice.

      // Try scoring
      if (numset == 1) {
        s[ix] = max(s[ix], 0 + s[index(one_less_than_acc)]);
      } else {
        s[ix] = max(s[ix], (1 << (numset - 2)) + s[index(one_less_than_acc)]);
      }
    }
  }

  return s[prod - 1];
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
      for (int j = 0; j < heights[i]; ++j) {
        int cij;
        cin >> cij;
        stacks[i].push_back(cij);
      }
    }

    cout << solve(heights, stacks) << endl;
  }
}
