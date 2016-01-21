#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <deque>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using namespace std;
using namespace CGAL;

// Inexact constructions doubles the speed. Otherwise we can't get maximum grade!
typedef Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;

struct segment {
  P p1;
  P p2;
};

struct triangle {
  segment s1;
  segment s2;
  segment s3;
};

void swap(P& a, P& b) {
  P t = a;
  a = b;
  b = t;
}

void swap(segment& s) {
  swap(s.p1, s.p2);
}

bool normal(triangle& t) {
  bool lt1 = left_turn(t.s1.p1, t.s1.p2, t.s2.p1);
  bool lt2 = left_turn(t.s2.p1, t.s2.p2, t.s3.p1);
  bool lt3 = left_turn(t.s3.p1, t.s3.p2, t.s1.p1);
  return lt1 && lt2 && lt3;
}

void normalise(triangle& t) {
  if (!left_turn(t.s1.p1, t.s1.p2, t.s2.p1)) { swap(t.s1); }
  if (!left_turn(t.s2.p1, t.s2.p2, t.s3.p1)) { swap(t.s2); }
  if (!left_turn(t.s3.p1, t.s3.p2, t.s1.p1)) { swap(t.s3); }
  // assert(normal(t));
}

bool within(P& p, triangle& t) {
  // assert(normal(t));
  bool lt1 = !right_turn(t.s1.p1, t.s1.p2, p);
  bool lt2 = !right_turn(t.s2.p1, t.s2.p2, p);
  bool lt3 = !right_turn(t.s3.p1, t.s3.p2, p);
  return lt1 && lt2 && lt3;
}

bool contained_in(segment& s, triangle& t) {
  // assert(normal(t));
  bool result = within(s.p1, t) & within(s.p2, t);
  return result;
}


int solve(int m, int n, vector<segment>& legs, vector<triangle>& triangles) {
  for (triangle& t: triangles) { normalise(t); }
  // for (triangle& t: triangles) { assert(normal(t)); }

  int segs = m - 1;

  vector<vector<int>> covered(n); // Key: triangle, Value: covered legs
  // O(nm)
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < segs; ++j) {
      if (contained_in(legs[j], triangles[i])) {
        covered[i].push_back(j);
      }
    }
  }

  int min_dist = INT_MAX;

  vector<int> covers(segs, 0); // covers[i]: number of traingles that cover segment i
  int nr_covered = 0;

  int i = -1;
  int j = 0;

  bool state = true; // True is looking right, false is taking left.
  // O(n^2)
  while (true) {
    if (j > i) { state = true; }
    if (j >= n) { break; }

    if (state) {
      // Go right, add a new triangle to the current set.
      if (i >= n - 1) {
        state = false;
        continue;
      }
      ++i;

      // The new triangle added is triangle i.
      int nr_covers = covered[i].size();
      // O(m)
      for (int k = 0; k < nr_covers; ++k) {
        int leg = covered[i][k];

        // If the number of covers was zero, it's going to be one so one more leg is covered.
        if (covers[leg] == 0) {
          ++nr_covered;
        }

        ++covers[leg];
      }
    } else {
      // The triangle removed is triangle j.
      int nr_covers = covered[j].size();
      // O(m)
      for (int k = 0; k < nr_covers; ++k) {
        int leg = covered[j][k];

        --covers[leg];

        // If the number of covers has become zero then one less leg is now covered.
        if (covers[leg] == 0) {
          --nr_covered;
        }
      }
      ++j;
    }

    if (nr_covered == segs) { // All covered
      min_dist = std::min(min_dist, i - j + 1);
      state = false; // Maybe less is fine too?
    } else {
      state = true; // Look for better situation.
    }
  }

  return min_dist;
}


int main() {
  ios_base::sync_with_stdio(false);

  int cases;
  cin >> cases;

  for (int c = 0; c < cases; ++c) {
    long m, n;
    cin >> m >> n;

    vector<P> ps;
    for (int i = 0; i < m; ++i) {
      long x, y;
      cin >> x >> y;
      P pi(x, y);
      ps.push_back(pi);
    }

    vector<segment> ss;
    for (int i = 0; i < m - 1; ++i) {
      segment s;
      s.p1 = ps[i];
      s.p2 = ps[i + 1];
      ss.push_back(s);
    }

    vector<triangle> ts;
    for (int i = 0; i < n; ++i) {
      P ps[6];
      for (int j = 0; j < 6; ++j) {
        long x, y;
        cin >> x >> y;
        ps[j] = P(x, y);
      }
      segment s1, s2, s3;
      s1.p1 = ps[0];
      s1.p2 = ps[1];
      s2.p1 = ps[2];
      s2.p2 = ps[3];
      s3.p1 = ps[4];
      s3.p2 = ps[5];

      triangle t;
      t.s1 = s1;
      t.s2 = s2;
      t.s3 = s3;
      ts.push_back(t);
    }
    cout << solve(m, n, ss, ts) << endl;
  }
}
