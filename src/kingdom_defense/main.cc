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
          property<edge_reverse_t, Traits::edge_descriptor>>>>  Graph;
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
  ReverseEdgeMap  &rev_edge;
};

struct loc {
  int stat;
  int req;
};

struct path {
  int from;
  int to;
  int mincap;
  int maxcap;
};

int p1(int i, int l, int p){
	return l + i;
}

int p2(int i, int l, int p){
	return l + i + p;
}

// We need to model 2 constrains:
// 1. minimum path usage
// 2. the number of soldiers needed
//
// Somehow we want to do both with a maxflow model
//
// For each original edge a -> d, we will construct seven edges:
//
//
// a -> b -> c -> d
// ^    |   -^    |
// |     \ /      |
// |      X       v
// src --/ \--> snk
//
// b -> will have capacity C-c. This models the fact that a minimum number of c soldiers must go before even one can go
// The flow that would have come from that is modeled with the edges from b to sink and the edge from src to c
// a -> b and c -> d have capacity C because that's the number that could go through a -> d
// src -> has capacity stat because that's how many soldiers are available at a
// d -> dnk has capacity req because that's how many solvers are needed at d
bool solve(int l, int p, vector<loc> locs, vector<path> paths) {
	Graph g(l + 2 * p);

  EdgeCapacityMap capacity = get(edge_capacity, g);
  ReverseEdgeMap rev_edge = get(edge_reverse, g);
  EdgeAdder ea(g,capacity, rev_edge);

  Vertex src = add_vertex(g);
  Vertex snk = add_vertex(g);

	for (int i = 0; i < p; ++i) {
		path& pp = paths[i];

    int a = pp.from;
    int b = p1(pp.from, l, p);
    int c = p2(pp.to, l, p);
    int d = pp.to;
		ea.addEdge(a, b, pp.maxcap);
    ea.addEdge(b, snk, pp.mincap);
    ea.addEdge(b, c, pp.maxcap - pp.mincap);
    ea.addEdge(src, c, pp.mincap);
    ea.addEdge(c, d, pp.maxcap);
	}

  for (int i = 0; i < l; ++i) {
    loc& ll = locs[i];
    ea.addEdge(src, i, ll.stat);
    ea.addEdge(i, snk, ll.req);
  }

  int flow = push_relabel_max_flow(g, src, snk);

	int mins = 0;
  for (int i = 0; i < p; ++i) {
    mins += paths[i].mincap;
  }

	int reqs = 0;
  for (int i = 0; i < l; ++i) {
    reqs += locs[i].req;
  }

  return flow == mins + reqs;
}

int main() {
  int cases;
  cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int l, p;
    cin >> l >> p;
    vector<loc> ls;
    for (int i = 0; i < l; ++i) {
      int g, d;
      cin >> g >> d;
      loc ll;
      ll.stat = g;
      ll.req = d;
      ls.push_back(ll);
    }
    vector<path> ps;
    for (int i = 0; i < p; ++i) {
      int f, t, minc, maxc;
      cin >> f >> t >> minc >> maxc;
      path pt;
      pt.from = f;
      pt.to = t;
      pt.mincap = minc;
      pt.maxcap = maxc;
      ps.push_back(pt);
    }
    if (solve (l, p, ls, ps)) {
      cout << "yes";
    } else {
      cout << "no";
    }
    cout << endl;
  }
}
