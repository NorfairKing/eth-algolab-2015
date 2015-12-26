#include <iostream>
#include <vector>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

using namespace boost;
using namespace std;

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
typedef graph_traits<Graph>::out_edge_iterator                  OutEdgeIt;

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


struct limb {
  int from;
  int to;
  int cost;
};

// This is a max-flow problem.
// Any max-flow represents a min-cut. Haha, pun intended.
pair<int, vector<int>> solve(int n, int m, vector<limb>& limbs) {
  Graph g(n);
  EdgeCapacityMap capacity = get(edge_capacity, g);
  ReverseEdgeMap rev_edge = get(edge_reverse, g);
  ResidualCapacityMap res_capacity = get(edge_residual_capacity, g);
  EdgeAdder ea(g,capacity, rev_edge);

  for (int i = 0; i < m; ++i) {
    limb& l = limbs[i];
    ea.addEdge(l.from, l.to, l.cost);
  }

  // We now find the best maxflow (minimum) in the graph
  // Me and my friend can pick any figure to start, so we try all sources and sinks
  int minflow = INT_MAX;
  int minsrc;
  int minsnk;

  // But, only the cut matters, not the entire flow.
  // No need to go through all possible sinks and sources.
  // Just the ones involving a fixed vertex (0 here).
  for (int i = 1; i < n; ++i) {
    int flow1 = push_relabel_max_flow(g, 0, i);
    if (flow1 < minflow) {
      minflow = flow1;
      minsrc = 0;
      minsnk = i;
    }
    int flow2 = push_relabel_max_flow(g, i, 0);
    if (flow2 < minflow) {
      minflow = flow2;
      minsrc = i;
      minsnk = 0;
    }
  }

  push_relabel_max_flow(g, minsrc, minsnk);

  queue<int> q;
  q.push(minsrc);

  vector<bool> visited(n, false);

  while(!q.empty()) {
    int from = q.front();
    q.pop();

    visited[from] = true;

    OutEdgeIt it, end;
    for(tie(it, end) = out_edges(from, g); it != end; ++it) {
      int to = target(*it, g);

      if (visited[to]) { continue; }

      // Don't go accross the cut.
      if (res_capacity[*it] == 0) { continue; }

      q.push(to);
    }
  }

  vector<int> fs;
  for (int i = 0; i < n; ++i) {
    if (visited[i]) { fs.push_back(i); }
  }

  return make_pair(minflow, fs);
}

int main() {
  int figures;
  cin >> figures;
  for (int f = 0; f < figures; ++f) {
    int n, m;
    cin >> n >> m;

    vector<limb> ls(m);
    for (int i = 0; i < m; ++i) {
      cin >> ls[i].from;
      cin >> ls[i].to;
      cin >> ls[i].cost;
    }

    pair<int, vector<int>> sol = solve(n, m, ls);
    cout << sol.first << endl;
    vector<int> fs = sol.second;
    int a = fs.size();
    cout << a;
    for (int i = 0; i < a; ++i) {
      cout << " " << fs[i];
    }
    cout << endl;
  }
}
