#include <iostream>
#include <vector>
#include <climits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;
using namespace CGAL;

typedef Exact_predicates_inexact_constructions_kernel K;
typedef Delaunay_triangulation_2<K>                   DTriang;
typedef DTriang::Edge_iterator                        EdgeIt;
typedef DTriang::Vertex_handle                        VertexH;
typedef K::Point_2                                    P;
typedef K::FT                                         F;

struct pos {
  int x;
  int y;
};

double ceil_to_double(const K::FT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

double solve(int n, vector<pos>& graypes) {
  vector<P> ps;
  for (int i = 0; i < n; ++i) {
    int x = graypes[i].x;
    int y = graypes[i].y;
    P p(x, y);
    ps.push_back(p);
  }

  DTriang t;
  t.insert(ps.begin(), ps.end());

  F min_dist = t.segment(t.finite_edges_begin()).squared_length(); // FIXME do this with optional
  for (EdgeIt e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    min_dist = std::min(min_dist, t.segment(e).squared_length());
  }
  min_dist = sqrt(min_dist * (100 * 100) / (2 * 2));

  return ceil_to_double(min_dist);
}

int main() {
  cout << setprecision(15);
  while (true) {
    int n;
    cin >> n;
    if (n == 0) { break; }

    vector<pos> gs(n);
    for (int i = 0; i < n; ++i) {
      cin >> gs[i].x;
      cin >> gs[i].y;
    }

    cout << solve(n, gs) << endl;
  }
}
