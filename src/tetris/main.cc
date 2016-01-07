#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS>            Traits;
typedef adjacency_list<
    vecS
  , vecS
  , directedS
  , no_property
  , property<edge_capacity_t, long,
      property<edge_residual_capacity_t, long,
        property<edge_reverse_t, Traits::edge_descriptor>>>>    Graph;
typedef property_map<Graph, edge_capacity_t>::type              EdgeCapacityMap;
typedef property_map<Graph, edge_residual_capacity_t>::type     ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type               ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor                  Vertex;
typedef graph_traits<Graph>::edge_descriptor                    Edge;
typedef graph_traits<Graph>::edge_iterator                      EdgeIt;

struct EdgeAdder {
  EdgeAdder(Graph & G, EdgeCapacityMap &capacity, ReverseEdgeMap &rev_edge)
    : G(G), capacity(capacity), rev_edge(rev_edge) {}
  void addEdge(int u, int v, long c) {
    Edge e, reverseE; bool success;
    tie(e, success) = add_edge(u, v, G);
    assert(success);
    tie(reverseE, success) = add_edge(v, u, G);
    assert(success);
    capacity[e] = c;
    capacity[reverseE] = 0;
    rev_edge[e] = reverseE;
    rev_edge[reverseE] = e;
  }
  Graph &G;
  EdgeCapacityMap &capacity;
  ReverseEdgeMap &rev_edge;
};

// This sounds like a max-flow problem.

// Let all integer positions in the set of endpoints of bricks be nodes.
//
//  position 0 -> bricks as edges with capacity 1 -> position w
//
// Because the wall has to be robust, make it a max capacity of 1 node
// by splitting it into 2 and drawing a capacity-1 edge between them.

int solve(int w, int n, vector<pair<int,int>>& bricks) {
  Graph g;
  EdgeCapacityMap capacity = get(edge_capacity, g);
  ReverseEdgeMap rev_edge = get(edge_reverse, g);
  EdgeAdder ea(g, capacity, rev_edge);

  // points[0] is the source and points[2*w-1] is the sink
  // points[2*i-1] is the left of location i and points[2*i] is the right

  // Second sample test case input
  // 0    1 2   3 4    5
  // |     |     |     |
  // |-----------------|
  //             |-----|
  //             |-----|
  // |-----|
  int t = 1 + 2 * (w - 1) + 1;
  vector<Vertex> points(t);
  for (int i = 0; i < t; ++i) {
    points[i] = add_vertex(g);
  }

  for (int i = 1; i < w; ++i) { // Edges within locations
    ea.addEdge(points[2 * i - 1], points[2 * i], 1); // Capacity one so that the wall is robust
  }

  for (int i = 0; i < n; ++i) {
    int left = bricks[i].first;
    int right = bricks[i].second;

    // Assign the right vertex number
    left = 2 * left;
    right = 2 * right - 1;

    ea.addEdge(points[left], points[right], 1);
  }

  int flow = push_relabel_max_flow(g, points[0], points[2 * w - 1]);
  return flow;
}

int main() {
  int cases;
  cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int w, n;
    cin >> w >> n;
    vector<pair<int,int>> bs(n);
    for (int i = 0; i < n; ++i) {
      int l, r;
      cin >> l;
      cin >> r;
      bs[i] = make_pair(min(l, r), max(l, r));
    }
    cout << solve(w, n, bs) << endl;
  }
}
