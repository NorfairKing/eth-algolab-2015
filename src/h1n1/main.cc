#include <iostream>
#include <vector>
#include <map>
#include <stack>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

using namespace std;
using namespace CGAL;

enum COLOR { Black = 2 , Grey = 1, White = 0 };

typedef Exact_predicates_inexact_constructions_kernel K;
typedef Delaunay_triangulation_2<K>                   DTriang;
typedef DTriang::Edge_iterator                        EdgeIt;
typedef DTriang::Face_iterator                        FaceIt;
typedef DTriang::Vertex_handle                        VertexH;
typedef DTriang::Face_handle                          FaceH;
typedef K::Point_2                                    P;
typedef K::Segment_2                                  S;
typedef K::FT                                         F;


struct person {
  int x;
  int y;
};

struct query {
  int x;
  int y;
  long d;
};


vector<bool> solve(int n, int m, vector<person>& infected, vector<query>& queries) {
  vector<P> ps;
  for (int i = 0; i < n; ++i) {
    int x = infected[i].x;
    int y = infected[i].y;
    P p(x, y);
    ps.push_back(p);
  }
  DTriang t;
  t.insert(ps.begin(), ps.end());
  
  vector<bool> res(m, false);
  for (int i = 0; i < m; ++i) {
    int x = queries[i].x;
    int y = queries[i].y;
    long d = queries[i].d;

    P query(x, y);

    stack<FaceH> s;

		P nearest = t.nearest_vertex(query)->point();
		if(squared_distance(query, nearest) < d) { continue; }

    FaceH startF = t.locate(query);
    s.push(startF);

    map<FaceH, COLOR> dfs_map; // White is the default.

		long min_sq_dist = 4 * d;
    while (!s.empty()) {
      FaceH cur = s.top();
      s.pop();

      if (t.is_infinite(cur)) {
        res[i] = true;
        break;
      }

      dfs_map[cur] = Black;

      // Each face has three points around them.
      // Between each two points, the query may get out.
      for(int e = 0; e < 3; ++e) {
        S seg = t.segment(cur, e);

        F dist = seg.squared_length();
        if (dist < min_sq_dist) { continue; } // Not far enough appart
        
        FaceH neighbor = cur->neighbor(e);
        // if (neighbor == cur) { continue; }
        if (dfs_map[neighbor] == Black
            || dfs_map[neighbor] == Grey) { continue; }

        s.push(neighbor);
        dfs_map[neighbor] = Grey;
      }
    }
  }

  return res;
}

int main() {
  while (true) {
    int n;
    cin >> n;
    if (n == 0) { break; }

    vector<person> is(n);
    for (int i = 0; i < n; ++i) {
      cin >> is[i].x;
      cin >> is[i].y;
    }

    int m;
    cin >> m;
    vector<query> qs(m);
    for (int i = 0; i < m; ++i) {
      cin >> qs[i].x;
      cin >> qs[i].y;
      cin >> qs[i].d;
    }

    vector<bool> solution = solve(n, m, is, qs);
    for (int i = 0; i < m; ++i) {
      if (solution[i]) {
        cout << "y";
      } else {
        cout << "n";
      }
    }
    cout << endl;
  }
}
