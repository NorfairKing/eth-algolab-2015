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
typedef K::Line_2 L;
typedef K::Segment_2 S;
typedef K::Triangle_2 T;

bool contained_in(S& segment, T& triangle) {
  if (!do_intersect(segment, triangle)) { return false; }
  auto o = intersection(segment, triangle);
  if (boost::get<P>(&*o)) {
    // cout << "it's a point." << endl;
    return false;
  } else if (const S* os = boost::get<S>(&*o)) {
    // cout << os->source() << ", " << os->target() << endl;
    if (*os == segment || *os == segment.opposite()) {
      return true;
    } else {
      return false;
    } 
  } else {
    cout << "Strange intersection you got theren buddy!.";
  }
  return false;
}

int solve(int m, int n, vector<S>& legs, vector<T>& triangles) {
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

    vector<S> ss;
    for (int i = 0; i < m - 1; ++i) {
      S si(ps[i], ps[i + 1]);
      //cout << si << endl;
      ss.push_back(si);
    }
    //cout << endl;


    vector<T> ts;
    for (int i = 0; i < n; ++i) {
      P ps[6];
      for (int j = 0; j < 6; ++j) {
        long x, y;
        cin >> x >> y;
        //cout << x << ", " << y << endl;
        ps[j] = P(x, y);
      }
      L l1(ps[0], ps[1]);
      L l2(ps[2], ps[3]);
      L l3(ps[4], ps[5]);

      //cout << l1 << endl;
      //cout << l2 << endl;
      //cout << l3 << endl;

      //cout << endl;

      auto i1 = intersection(l1, l2);
      const P* p1 = boost::get<P>(&*i1);
      auto i2 = intersection(l2, l3);
      const P* p2 = boost::get<P>(&*i2);
      auto i3 = intersection(l1, l3);
      const P* p3 = boost::get<P>(&*i3);

      T t(*p1, *p2, *p3);
      //cout << t << endl;
      ts.push_back(t);
      //cout << endl;
    }
    //cout << endl;

    cout << solve(m, n, ss, ts) << endl;
  }
}
