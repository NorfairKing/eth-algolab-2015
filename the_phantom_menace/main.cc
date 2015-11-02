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

struct lnk { // Somehow I can't name this link without getting compile-errors
  int from;
  int to;
};

int solve(int n, int m, int s, int d, vector<lnk>& links, vector<int>& starts, vector<int>& ends){
  Graph g;
  EdgeCapacityMap capacity = get(edge_capacity, g);
  ReverseEdgeMap rev_edge = get(edge_reverse, g);
  EdgeAdder ea(g, capacity, rev_edge);

  vector<Vertex> ins;
  vector<Vertex> outs;

  for (int i = 0; i < n; ++i) {
    Vertex incoming = add_vertex(g);
    Vertex outgoing = add_vertex(g);
    ins.push_back(incoming);
    outs.push_back(outgoing);

    ea.addEdge(incoming, outgoing, 1);

    // cout << incoming << ", " << outgoing << endl;
  }

  for (int i = 0; i < m; ++i) {
    lnk& l = links[i];
    Vertex f = outs[l.from];
    Vertex t = ins[l.to];
    ea.addEdge(f, t, 1);

    // cout << f << " -> " << t << endl;
  }

  Vertex src = add_vertex(g);
  Vertex snk = add_vertex(g);

  for (int i = 0; i < s; ++i) {
    Vertex t = ins[starts[i]];
    ea.addEdge(src, t, 1);
    // cout << src << " -> " << t << endl;
  }
  for (int i = 0; i < d; ++i) {
    Vertex f = outs[ends[i]];
    ea.addEdge(f, snk, 1);
    // cout << f << " -> " << snk << endl;
  }

  int flow = push_relabel_max_flow(g, src, snk);
  return flow;
}

int main () {
  int cases;
  cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int n, m, s, d;
    cin >> n >> m >> s >> d;

    vector<lnk> ls(m);
    for (int i = 0; i < m; ++i) {
      int f, t;
      cin >> f >> t;
      ls[i].from = f;
      ls[i].to = t;
    }

    vector<int> ss;
    for (int i = 0; i < s; ++i) {
      int sl;
      cin >> sl;
      ss.push_back(sl);
    }

    vector<int> ds;
    for (int i = 0; i < d; ++i) {
      int dl;
      cin >> dl;
      ds.push_back(dl);
    }
    cout << solve(n, m, s, d, ls, ss, ds) << endl;
  }
}
