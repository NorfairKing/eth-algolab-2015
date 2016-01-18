#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

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
typedef graph_traits<Graph>::edge_iterator                      EdgeIt; // Iterator

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

typedef int shop;
struct street {
  int from;
  int to;
};

// He can always go back the way he came.
bool solve(int n, int m, int s, vector<shop> shops, vector<street> streets) {
  Graph g(n);
  EdgeCapacityMap capacity = get(edge_capacity, g);
  ReverseEdgeMap rev_edge = get(edge_reverse, g);
  EdgeAdder ea(g,capacity, rev_edge);

  Vertex src = add_vertex(g);
  Vertex snk = add_vertex(g);

  ea.addEdge(src, 0, s);
  for (int i = 0; i < s; ++i) {
    ea.addEdge(shops[i], snk, 1);
  }
  for (int i = 0; i < m; ++i) {
    street& ss = streets[i];
    int of = ss.from;
    int ot = ss.to;
    ea.addEdge(of, ot, 1);
    ea.addEdge(ot, of, 1);
  }

  int flow = push_relabel_max_flow(g, src, snk);

  return flow == s;
}

int main() {
  int cases;
  cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int n, m, s;
    cin >> n >> m >> s;
    vector<shop> ss;
    for (int i = 0; i < s; ++i) {
      int si;
      cin >> si;
      ss.push_back(si);
    }
    vector<street> sts;
    for (int i = 0; i < m; ++i) {
      int f, t;
      cin >> f >> t;
      street st;
      st.from = f;
      st.to = t;
      sts.push_back(st);
    }

    if (solve(n, m, s, ss, sts)) {
      cout << "yes";
    } else {
      cout << "no";
    }
    cout << endl;
  }
}
