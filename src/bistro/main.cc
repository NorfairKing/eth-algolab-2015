#include <iostream>
#include <vector>

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



struct rest {
  int x;
  int y;
};

vector<double> solve(int n, int m, vector<rest>& old, vector<rest> nwr) {
  vector<P> ps;
  for (int i = 0; i < n; ++i) {
    int x = old[i].x;
    int y = old[i].y;
    P p(x, y);
    ps.push_back(p);
  }
  DTriang t;
  t.insert(ps.begin(), ps.end());

  vector<double> res;
  for (int i = 0; i < m; ++i) {
    int x = nwr[i].x;
    int y = nwr[i].y;
    P q(x, y);

    P nearest = t.nearest_vertex(q)->point();
    res.push_back(to_double(squared_distance(q, nearest)));
  }

  return res;
}

int main() {
  cout << fixed << setprecision(0);
  while (true) {
    int n;
    cin >> n;
    if (n == 0) { break; }

    vector<rest> ors(n);
    for (int i = 0; i < n; ++i) {
      cin >> ors[i].x;
      cin >> ors[i].y;
    }

    int m;
    cin >> m;
    vector<rest> nrs(m);
    for (int i = 0; i < m; ++i) {
      cin >> nrs[i].x;
      cin >> nrs[i].y;
    }

    vector<double> solution = solve(n, m, ors, nrs);
    for (int i = 0; i < m; ++i) {
      cout << solution[i] << endl;
    }
  }
}
