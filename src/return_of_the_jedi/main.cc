#include <iostream>
#include <vector>
#include <climits>

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

// #include <boost/tuple/tuple.hpp>  // tuples::ignore

using namespace std;
using namespace boost;

typedef adjacency_list<
  vecS,
  vecS,
  undirectedS,
  no_property,
  property<edge_weight_t, int>>                   Graph;
typedef graph_traits<Graph>::vertex_descriptor    Vertex;
typedef graph_traits<Graph>::edge_descriptor      Edge;
typedef graph_traits<Graph>::edge_iterator        EdgeIt;
typedef property_map<Graph, edge_weight_t>::type  WeightMap;


int solve(int n, int i, vector<vector<int>> dists) {
  Graph g(n);
  WeightMap ws = get(edge_weight, g);

  for (int j = 0; j < n - 1 - 1; ++j) {
    for (int k = 0; k < n - j - 1; ++k) {
      Edge e; bool success;
      tie(e, success) = add_edge(j, j + k + 1, g);
      assert(success);
      ws[e] = dists[j][k];
    }
  }

  vector<Vertex> p(n);
  prim_minimum_spanning_tree(g, &p[0]);

  int weight = 0;
  vector<vector<bool>> taken(n, vector<bool>(n, false));
  for (int from = 0; from < n; ++from) {
    int to = p[from];
    Edge e; bool success;
    tie(e, success) = edge(from, to, g);

    if (! success) { continue; } // No parent

    // taken[min(from, to)][max(from, to)] = true;
    taken[from][to] = taken[to][from] = true;
    weight += ws[e];
  }

  cout << "vertices: " << n << endl;
  cout << "weight: " << weight << endl;
  for (int j = 0; j < n; ++j) {
    for (int k = 0; k < n; ++k) {
      cout << taken[j][k] << " ";
    }
    cout << endl;
  }

  for (int from = 0; from < n; ++from) {
    for (int to = 0; to < n; ++to) {
      if (!taken[from][to]) { continue; } // already have this one.
      
      Edge e; bool success;
      tie(e, success) = edge(from, to, g);
      assert(success);

      cout << from << " -> " << to  << " :  " << ws[e] << endl;
    }
  }


  return 5;
}

int main() {
  int cases;
  cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int n, i;
    cin >> n >> i;

    vector<vector<int>> ds(n - 1);
    for (int j = 1; j <= n - 1; ++j) {
      for (int k = 1; k <= n - j; ++k) {
        int cost;
        cin >> cost;
        ds[j - 1].push_back(cost);
      }
    }
    cout << solve(n, i, ds) << endl;
  }
}
