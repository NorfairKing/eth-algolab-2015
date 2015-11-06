#include <iostream>
#include <vector>
#include <boost/optional.hpp>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/nearest_neighbor_delaunay_2.h>

using namespace std;
using namespace boost;
using namespace CGAL;

typedef Exact_predicates_inexact_constructions_kernel K;
typedef Delaunay_triangulation_2<K>                   DTriang;
typedef DTriang::Edge_iterator                        EdgeIt;
typedef DTriang::Vertex_iterator                      VertexIt;
typedef DTriang::Vertex_handle                        VertexH;
typedef DTriang::Point								    					  DP;
typedef K::Point_2                                    P;
typedef K::Segment_2                                  S;
typedef K::FT                                         F;

struct pos {
  int x;
  int y;
};

struct res {
  double f;
  double m;
  double l;
};

double ceil_to_double(const K::FT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

optional<F>& mini(optional<F>& d1, optional<F>& d2) {
  if (d1 && ! d2) { return d1; }
  if (d2 && ! d1) { return d2; }
  if (d1.get() < d2.get()) {
    return d1;
  } else {
    return d2;
  }
}

// t^2 + 0.5 = x
// t = sqrt(x - 0.5)
const F INITIAL_RADIUS(0.5);
const F Zero(0);
double hours(F& dist) {
  F x = sqrt(dist);
  x -= INITIAL_RADIUS;
  if (x <= Zero) {
    return 0;
  } else {
    return ceil_to_double(sqrt(x));
  }
}

res solve(int n, int l, int b, int r, int t, vector<pos> ps) {

  // Construct rectangle segments
  P tl(l, t);
  P bl(l, b);
  P tr(r, t);
  P br(r, b);
  P corners[4];
  corners[0] = tl;
  corners[1] = bl;
  corners[2] = br;
  corners[3] = tr;

  S rect[4];
  for (int i = 0; i < 4; ++i) {
    S s(corners[i], corners[(i + 1) % 4]);
    rect[i] = s;
  }

  vector<P> bs;
  for (int i = 0; i < n; ++i) {
    int x = ps[i].x;
    int y = ps[i].y;
    P p(x, y);
    bs.push_back(p);
  }

  DTriang trian;
  trian.insert(bs.begin(), bs.end());

	// keep track of the distances for each bacteria
	map<DP, optional<F>> sqdist;

	// calculate initial sqdistance: sqdistance between the bacteria and the nearest dish boundary
	for(VertexIt vertex_iter = trian.finite_vertices_begin();
			vertex_iter != trian.finite_vertices_end();
		++vertex_iter) {
		DP p = vertex_iter->point();

    for (int j = 0; j < 4; ++j) {
      optional<F> d = squared_distance(p, rect[j]);
      sqdist[p] = mini(sqdist[p], d);
    }
	}


  for(EdgeIt it = trian.finite_edges_begin();
      it != trian.finite_edges_end();
      ++it) {

    VertexH v1 = it->first->vertex(trian.cw (it->second));
    VertexH v2 = it->first->vertex(trian.ccw(it->second));

    DP p1 = v1->point();
    DP p2 = v2->point();
    optional<F> d = squared_distance(p1, p2) / (2 * 2);
    sqdist[p1] = mini(sqdist[p1], d);
    sqdist[p2] = mini(sqdist[p2], d);
  }

  vector<F> sqdistances;
  for(auto it = sqdist.begin();
      it != sqdist.end();
      ++it) {
    F& d = it->second.get();
    sqdistances.push_back(d);
  }

  sort(sqdistances.begin(), sqdistances.end());

  res result;
  result.f = hours(sqdistances[0]);
  result.m = hours(sqdistances[n/2]);
  result.l = hours(sqdistances[n-1]);
  return result;
}

int main() {
  while (true) {
    int n;
    cin >> n;
    if (n == 0) { break; }
    
    int l, b, r, t;
    cin >> l >> b >> r >> t;

    vector<pos> ps;
    for (int i = 0; i < n; ++i) {
      int x, y;
      cin >> x;
      cin >> y;
      pos p;
      p.x = x;
      p.y = y;
      ps.push_back(p);
    }
    res sol = solve(n, l, b, r, t, ps);
    cout << sol.f << " " << sol.m << " " << sol.l << endl;
  }
}
