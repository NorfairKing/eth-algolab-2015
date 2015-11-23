#include <iostream>
#include <vector>
#include <climits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/basic.h>

using namespace std;
using namespace boost;
using namespace CGAL;

typedef adjacency_list_traits<vecS, vecS, directedS>        Traits;
typedef adjacency_list<
  vecS,
  vecS,
  directedS,
  no_property,
  property<edge_capacity_t, long,
    property<edge_residual_capacity_t, long,
      property<edge_reverse_t, Traits::edge_descriptor,
        property <edge_weight_t, long>>>>>                  Graph;
typedef property_map<Graph, edge_capacity_t>::type          EdgeCapacityMap;
typedef property_map<Graph, edge_weight_t >::type           EdgeWeightMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type           ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor              Vertex;
typedef graph_traits<Graph>::edge_descriptor                Edge;
typedef graph_traits<Graph>::out_edge_iterator              OutEdgeIt;

typedef Exact_predicates_exact_constructions_kernel         K;
typedef K::Point_2                                          P;
typedef K::Segment_2                                        S;
typedef K::FT                                               F;

typedef Delaunay_triangulation_2<K>                   DTriang;
typedef DTriang::Edge_iterator                        EdgeIt;
typedef DTriang::Vertex_handle                        VertexH;


struct EdgeAdder {
  EdgeAdder(Graph & G, EdgeCapacityMap &capacity, EdgeWeightMap &weight, ReverseEdgeMap &rev_edge) 
    : G(G), capacity(capacity), weight(weight), rev_edge(rev_edge) {}

  void addEdge(int u, int v, long c, long w) {
    Edge e, reverseE;
    tie(e, tuples::ignore) = add_edge(u, v, G);
    tie(reverseE, tuples::ignore) = add_edge(v, u, G);
    capacity[e] = c;
    weight[e] = w;
    capacity[reverseE] = 0;
    weight[reverseE] = -w;
    rev_edge[e] = reverseE;
    rev_edge[reverseE] = e;
  }
  Graph &G;
  EdgeCapacityMap &capacity;
  EdgeWeightMap &weight;
  ReverseEdgeMap  &rev_edge;
};

struct part {
  int x;
  int y;
  int d;
};

struct spoint {
  int x;
  int y;
};

struct hunter {
  int x;
  int y;
};

bool solve(int a, int s, int b, int e,
    vector<part>& particles,
    vector<spoint>& spoints,
    vector<hunter>& hunters) {

  // Build Delaunay Triangulation of hunters' locations
  vector<P> bhs;
  for (int i = 0; i < b; ++i) {
    int x = hunters[i].x;
    int y = hunters[i].y;
    P p(x, y);
    bhs.push_back(p);
  }
  DTriang t;
  t.insert(bhs.begin(), bhs.end());

  // For each shooting point, find the maximum radius that a blast can have/
  vector<long> maxsqradius(s, LONG_MAX);
  if (b > 0) { // No bounty hunters, no closest ones. Would otherwise segfault
    for (int i = 0; i < a; ++i) {
      int x = spoints[i].x;
      int y = spoints[i].y;
      P q(x, y);
      P nearest = t.nearest_vertex(q)->point();
      maxsqradius[i] = to_double(squared_distance(q, nearest));
    }
  }
  
  Graph g(a + s);
  EdgeCapacityMap capacity = get(edge_capacity, g);
  EdgeWeightMap weight = get(edge_weight, g);
  ReverseEdgeMap rev_edge = get(edge_reverse, g);
  EdgeAdder ea(g, capacity, weight, rev_edge);
          
  Vertex src = add_vertex(g);
  Vertex snk = add_vertex(g);

  // Edges from the source to every shooting point with unlimited capacity and cost 0.
  for (int i = 0; i < s; ++i) {
    ea.addEdge(src, i, e, 0); // cap unlimited (just put e because it's an upper bound), cost 0
  }

  // Edges from shooting points to asteroids, unlimited capacity, cost equal to square of distance.
  for (int i = 0; i < s; ++i) { // point
    long maxsqrad = maxsqradius[i];

    int fromx = spoints[i].x;
    int fromy = spoints[i].y;
    P from(fromx, fromy);

    for (int j = 0; j < a; ++j) { // asteroid
      int tox = particles[j].x;
      int toy = particles[j].y;
      P to(tox, toy);

      long dd = to_double(squared_distance(from, to)); // Will be an integer anyway.

      // cout << dd << " " << maxsqrad << endl;
      if (dd >= maxsqrad) { continue; } // Too close, bounty hunters would notice.

      ea.addEdge(i, s + j, e, dd); // cap e, cost distance squared.
    }
  }

  // Edges from asteroids to sink with capacity equal to the density of the particle.
  for (int j = 0; j < a; ++j) {
    ea.addEdge(s + j, snk, particles[j].d, 0); // cap density, cost 0
  }

  // Find flow and cost
  int flow = push_relabel_max_flow(g, src, snk);
  cycle_canceling(g);
  long cost = find_flow_cost(g);

  int total_density = 0;
  for (int j = 0; j < a; ++j) {
    total_density += particles[j].d;
  }

  cout << "flow: " << flow << ", density: " << total_density << endl;;
  cout << "cost: " << cost << ", energy: " << e << endl;
  
  return flow == total_density && cost <= e;
}

int main() {
  int cases;
  cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int a, s, b;
    cin >> a >> s >> b;

    int e;
    cin >> e;

    vector<part> ps(a);
    for (int i = 0; i < a; ++i) {
      cin >> ps[i].x;
      cin >> ps[i].y;
      cin >> ps[i].d;
    }

    vector<spoint> ss(s);
    for (int i = 0; i < s; ++i) {
      cin >> ss[i].x;
      cin >> ss[i].y;
    }

    vector<hunter> hs(b);
    for (int i = 0; i < b; ++i) {
      cin >> hs[i].x;
      cin >> hs[i].y;
    }
    if (solve(a, s, b, e, ps, ss, hs)) {
      cout << "y";
    } else {
      cout << "n";
    }
    cout << endl;
  }
}
