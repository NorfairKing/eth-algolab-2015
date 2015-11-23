#include <iostream>
#include <vector>
#include <climits>
#include <queue>

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

// #include <boost/tuple/tuple.hpp>  // tuples::ignore

using namespace std;
using namespace boost;

// typedef adjacency_list<
//   vecS,
//   vecS,
//   undirectedS,
//   no_property,
//   property<edge_weight_t, int>>                   Graph;
typedef adjacency_matrix<
  undirectedS,
  no_property,
  property<edge_weight_t, int>,
  no_property>                                    Graph;
typedef graph_traits<Graph>::vertex_descriptor    Vertex;
typedef graph_traits<Graph>::edge_descriptor      Edge;
typedef graph_traits<Graph>::edge_iterator        EdgeIt;
typedef property_map<Graph, edge_weight_t>::type  WeightMap;

enum color { WHITE, GRAY, BLACK };

int solve(int n, int i, vector<vector<int>> dists) {
  Graph g(n);
  WeightMap ws = get(edge_weight, g);

  for (int j = 0; j < n - 1; ++j) {
    for (int k = 0; k < n - j - 1; ++k) {
      Edge e; bool success;
      tie(e, success) = add_edge(j, j + k + 1, g);
      assert(success);
      ws[e] = dists[j][k];
    }
  }

  vector<Vertex> p(n);
  prim_minimum_spanning_tree(g, &p[0]);
  for (int j = 0; j < n; ++j) {
    cout << j << ": " << p[j] << endl;
  }
  cout << endl;

  int total_weight = 0;
  vector<vector<int>> weight(n, vector<int>(n, 0));
  for (int from = 0; from < n; ++from) {
    int to = p[from];
    Edge e; bool success;
    tie(e, success) = edge(from, to, g);

    if (! success) { continue; } // No parent, not on the tree.

    weight[from][to] = weight[to][from] = ws[e];
    total_weight += ws[e];
  }

  // Add an edge that is not on the MST
  // Then remove the heaviest edge on the cycle that was just formed.
  Edge adding;
  bool found = false;
  for (int from = 0; from < n; ++from) {
    for (int to = from + 1; to < n; ++to) {
      int pf = p[from];
      int pt = p[to];
      if (to == pf || from == pt) { continue; } // Already in the tree.
      bool success;
      tie(adding, success) = edge(from, to, g);
      if (!success) { continue; }
      cout << from << " <> " << to << endl;
      total_weight += ws[adding];
      found = true;
      break;
    }
  }
  if (!found) { return total_weight; }

  // BFS to find the cycle from target to itsself
  // Keep the largest weight on it.
  vector<color> visited(n, WHITE);
  vector<vector<bool>> taken(n, vector<bool>(n, false));

  std::queue<Vertex> q;
  Vertex start = source(adding, g);
  q.push(start);

  while (!q.empty()) {
    Vertex cur = q.front();
    q.pop();

    cout << "cur: " << cur << endl;
  }

  // vector<vector<bool>> taken(n, vector<bool>(n, false));
  // vector<vector<int>> weight(n, vector<int>(n, 0));

  // for (int from = 0; from < n; ++from) {
  //   int to = p[from];
  //   Edge e; bool success;
  //   tie(e, success) = edge(from, to, g);

  //   if (! success) { continue; } // No parent

  //   // taken[min(from, to)][max(from, to)] = true;
  //   taken[from][to] = taken[to][from] = true;
  //   weight[from][to] = weight[to][from] = ws[e];
  //   total_weight += ws[e];
  // }

  // cout << "vertices: " << n << endl;
  // cout << "weight: " << total_weight << endl;
  // for (int j = 0; j < n; ++j) {
  //   for (int k = 0; k < n; ++k) {
  //     cout << taken[j][k] << " ";
  //   }
  //   cout << endl;
  // }

  // for (int from = 0; from < n; ++from) {
  //   for (int to = from + 1; to < n; ++to) {
  //     if (!taken[from][to]) { continue; } // already have this one.
  //     
  //     Edge e; bool success;
  //     tie(e, success) = edge(from, to, g);
  //     assert(success);

  //     cout << from << " -> " << to  << " :  " << ws[e] << endl;

  //     // Find out which collumn is now empty. It's either the from or the to collumn, or both.
  //     // If none of them would now be empty, it wouldn't be a tree.
  //     int from_co
  //     bool from_empty = true;
  //     for (int k = 0; k < n; ++k) {
  //       if (k == to) { continue; }
  //       if (taken[k][from]) {
  //         from_empty = false;
  //         break;
  //       }
  //     }
  //     bool to_empty = true;
  //     for (int k = 0; k < n; ++k) {
  //       if (k == from) { continue; }
  //       if (taken[k][to]) {
  //         to_empty = false;
  //         break;
  //       }
  //     }
  //     cout << "from_e: " << from_empty << "  " << "to_empty: " << to_empty << endl;
  //   }
  // }




  return total_weight;
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
  cout << endl;
  }
}
