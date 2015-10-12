#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
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
  for (int i = 0; i < segs; ++i) {
    for (int j = i; j < segs; ++j) {
      // maps from i to j inclusive.


      vector<bool> found(segs);
      for (int k = 0; k < n; ++k) {
        for (int l = i; l <= j; ++l) {
          if (contained_in(legs[l], triangles[k])) {
            found[l] = true;
          }
        }
      }
      for (int k = 0; k < segs; ++k) {
        if (!found[k]) {
          //cout << "false" << endl;
          continue;
        } else {
          //cout << "true" << endl;
          int dist = j - i + 1;
          min_dist = std::min(dist, min_dist);
        }
      }
      //cout << endl;
    }
  }

  return min_dist;
}

int main() {
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
