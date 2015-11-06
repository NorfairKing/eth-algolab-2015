#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <deque>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using namespace std;
using namespace CGAL;

typedef Exact_predicates_exact_constructions_kernel K;
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
  assert(normal(t));
}

bool within(P& p, triangle& t) {
  assert(normal(t));
  bool lt1 = !right_turn(t.s1.p1, t.s1.p2, p);
  bool lt2 = !right_turn(t.s2.p1, t.s2.p2, p);
  bool lt3 = !right_turn(t.s3.p1, t.s3.p2, p);
  // cout << t.s1.p1 << ", " << t.s1.p2 << ", " << p << endl;
  // cout << t.s2.p1 << ", " << t.s2.p2 << ", " << p << endl;
  // cout << t.s3.p1 << ", " << t.s3.p2 << ", " << p << endl;
  // cout << lt1 << " " << lt2 << " " << lt3 << endl;
  return lt1 && lt2 && lt3;
}

bool contained_in(segment& s, triangle& t) {
  assert(normal(t));
  bool result = within(s.p1, t) & within(s.p2, t);
  // cout << result << endl;
  return result;
}


int solve(int m, int n, vector<segment>& legs, vector<triangle>& triangles) {
  for (triangle& t: triangles) { normalise(t); }
  for (triangle& t: triangles) { assert(normal(t)); }

  int segs = m - 1;
  int min_dist = INT_MAX;

  int i = -1;
  int j = 0;

  bool state = true; // True is looking right, false is taking left.
  vector<deque<int>> covering(segs); // triangles covering that segment
  while (true) {
    if (j > i) { state = true; }
    if (j >= n) { break; }
    if (state) {
      //cout << "state=true" << endl;
      if (i >= n - 1) {
        state = false;
        continue;
      }
      ++i;
      for (int k = 0; k < segs; ++k) {
        if (contained_in(legs[k], triangles[i])) {
          covering[k].push_back(i);
        }
      }
    } else {
      for (int k = 0; k < segs; ++k) {
        if (contained_in(legs[k], triangles[j])) {
          assert(covering[k].front() == j);
          covering[k].pop_front();
        }
      }
      ++j;
    }

    //cout << "i: " << i << "  j: " << j << endl;
    bool all_covered = true;
    for (int k = 0; k < segs; ++k) {
      if (covering[k].empty()) { // not all covered
        all_covered = false;
        break;
      }
  
      //cout << k << ": ";
      //for (auto it = covering[k].begin(); it < covering[k].end(); ++it){
      //  cout << *it << " ";
      //}
      //cout << endl;
    }
    //cout << endl;
    //cout << "i: " << i << "  j: " << j << "  segs: " << segs << "  n: " << n << endl;
    if (all_covered) {
      //cout << "all covered" << endl;
      // all covered
      min_dist = std::min(min_dist, i - j + 1);
      //cout << "setting min_dist to: " << min_dist << endl;
      state = false; // maybe less is fine too?
      //cout << endl;
    } else {
      //cout << " not all covered " << endl;
      state = true; // look for better situation.
    }
  }

  return min_dist;
}


int main() {
  std::ios_base::sync_with_stdio(false);

  int cases;
  cin >> cases;

  for (int c = 0; c < cases; ++c) {
    long m, n;
    cin >> m >> n;

    vector<P> ps;
    for (int i = 0; i < m; ++i) {
      long x, y;
      cin >> x >> y;
      //cout << x << ", " << y << endl;
      P pi(x, y);
      ps.push_back(pi);
    }
    //cout << endl;

    vector<segment> ss;
    for (int i = 0; i < m - 1; ++i) {
      segment s;
      s.p1 = ps[i];
      s.p2 = ps[i + 1];
      //cout << si << endl;
      ss.push_back(s);
    }
    //cout << endl;


    vector<triangle> ts;
    for (int i = 0; i < n; ++i) {
      P ps[6];
      for (int j = 0; j < 6; ++j) {
        long x, y;
        cin >> x >> y;
        //cout << x << ", " << y << endl;
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

      //cout << endl;
    }
    //cout << endl;

    cout << solve(m, n, ss, ts) << endl;
  }
}
