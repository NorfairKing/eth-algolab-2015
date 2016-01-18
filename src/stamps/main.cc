#include <iostream>
#include <vector>
#include <boost/optional/optional_io.hpp>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>


using namespace std;
using namespace CGAL;

#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>
typedef Gmpzf ET;
#else
#include <CGAL/MP_Float.h>
typedef MP_Float ET;
#endif

typedef Quadratic_program<ET>                       Program;
typedef Quadratic_program_solution<ET>              Solution;
typedef Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2                                  P;
typedef K::Segment_2                                S;
typedef K::FT                                       F;


struct lamp {
  int x;
  int y;
};

struct stamp {
  int x;
  int y;
  int maxint;
};

struct wall {
  int a;
  int b;
  int c;
  int d;
};

bool solve(int l, int s, int w
         , vector<lamp>& lamps
         , vector<stamp>& stamps
         , vector<wall>& walls) {

  vector<P> ls;
  for (int i = 0; i < l; ++i) {
    lamp& la = lamps[i];
    P p(la.x, la.y);
    ls.push_back(p);

    // cout << p << endl;
  }
  
  vector<P> ss;
  for (int i = 0; i < s; ++i) {
    stamp& st = stamps[i];
    P p(st.x, st.y);
    ss.push_back(p);

    // cout << p << endl;
  }

  vector<S> ws;
  for (int i = 0; i < w; ++i) {
    wall& wa = walls[i];
    P p1(wa.a, wa.b);
    P p2(wa.c, wa.d);
    S seg(p1, p2);
    ws.push_back(seg);

    // cout << seg << endl;
  }

  // This is a quadratic programming problem!
  // There will be l variables, each between 1 and 2^12
  // the variables are power_i where i is the index of a lamp.

  Program p(SMALLER
      , true, 1 // Lower bound 1
      , true, (1 << 12)); // Upper bound 2^12

  // We will minimize the total luminocity
  // really we just want something that satisfies the constraints
  // so we'll just pick any objective function.
  // We don't need an objective function!

  // There will be constraints to model these things:
  // if there is a wall in between a lamp and stamp, there is no constraint
  // on the intensity of that lamp for that stamp given by that wall
  //
  // if there is, there is a quadratic constraint on the intensity of that lamp
  // given by that stamp.
  
  // Model:
  //
  //  forall j in {j = 0} .. {j = s - 1}
  //    sum_{i = 0}^{l - 1} intensity_from_lamp_i_for_stamp_j <= mj
  //    sum_{i = 0}^{l - 1} intensity_from_lamp_i_for_stamp_j >= 1
  //  where intensity_from_lamp_i_for_stamp_j = power_i * 1 / sqrt(dist)
  for (int j = 0; j < s; ++j) {
    for (int i = 0; i < l; ++i) {
      S r(ss[j], ls[i]);

      bool blocked = false;
      for (int k = 0; k < w; ++k) {
        if (do_intersect(ws[k], r)) {
          blocked = true;
          break;
        }
      }

      if (blocked) { continue; }

      F invdist = 1 / r.squared_length();
      double idist = to_double(invdist);
      p.set_a(i, j    , idist);
      p.set_a(i, j + s, idist);
    }
    p.set_r(j    , SMALLER);
    p.set_b(j    , stamps[j].maxint);

    p.set_r(j + s, LARGER);
    p.set_b(j + s, 1);
  }


  Solution sol = solve_linear_program(p, ET());
  return !sol.is_infeasible();
}

int main() {
  int cases;
  cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int l, s, w;
    cin >> l >> s >> w;
    // cout << l << " " << s << " " << w << endl;

    vector<lamp> ls(l);
    for (int i = 0; i < l; ++i) {
      cin >> ls[i].x;
      cin >> ls[i].y;
    }

    vector<stamp> ss(s);
    for (int i = 0; i < s; ++i) {
      cin >> ss[i].x;
      cin >> ss[i].y;
      cin >> ss[i].maxint;
    }

    vector<wall> ws(w);
    for (int i = 0; i < w; ++i) {
      cin >> ws[i].a;
      cin >> ws[i].b;
      cin >> ws[i].c;
      cin >> ws[i].d;
    }

    if (solve(l, s, w, ls, ss, ws)) {
      cout << "yes";
    } else {
      cout << "no";
    }
    cout << endl;
  }
}
