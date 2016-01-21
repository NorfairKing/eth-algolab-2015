#include <iostream>
#include <vector>
#include <climits>
#include <stack>

#include <boost/tuple/tuple.hpp>
// #include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

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
typedef graph_traits<Graph>::out_edge_iterator    OutEdgeIt;
typedef property_map<Graph, edge_weight_t>::type  WeightMap;

struct node {
  Vertex v;
  node* parent;
  int heaviest;
};

int solve(int n, int i, vector<vector<int>> dists) {
  Graph g(n);
  WeightMap ws = get(edge_weight, g);

  // Add all possible edges. (it's a clique!)
  for (int j = 0; j < n - 1; ++j) {
    for (int k = 0; k < n - j - 1; ++k) {
      Edge e; bool success;
      tie(e, success) = add_edge(j, j + k + 1, g);
      assert(success);
      ws[e] = dists[j][k];
    }
  }

  // Find the minimum spanning tree
  vector<Vertex> p(n);
  prim_minimum_spanning_tree(g, &p[0]);

  vector<vector<int>> neighbors(n); // at most 2 neighbors of course.

  int total_weight = 0;
  vector<vector<int>> weight(n, vector<int>(n, 0));
  for (int from = 0; from < n; ++from) {
    int to = p[from];
    Edge e; bool success;
    tie(e, success) = edge(from, to, g);

    if (! success) { continue; } // No parent, not on the tree.

    weight[from][to] = weight[to][from] = ws[e];
    total_weight += ws[e];

    neighbors[from].push_back(to);
    neighbors[to].push_back(from);
  }

  // Add an edge that is not on the MST
  // Then remove the heaviest edge on the cycle that was just formed.
  // Keep the best of the resulting trees.

  vector<Edge> additions;

  for (int from = 0; from < n; ++from) {
    for (int to = from + 1; to < n; ++to) {
      int pf = p[from];
      int pt = p[to];
      if (to == pf || from == pt) { continue; } // Already in the tree.

      Edge adding; bool success;
      tie(adding, success) = edge(from, to, g);
      assert(success); // All possible edges should be in the graph because it's a clique.

      additions.push_back(adding);
      // cout << from << " <> " << to << endl;
      // total_weight += ws[adding];
    }
  }

  int minweight = INT_MAX;

  int en = additions.size();
  for (int ei = 0; ei < en; ++ei) {
    Edge adding = additions[ei];

    int w = total_weight +  ws[adding];

    stack<node*> q;
    Vertex sv = source(adding, g);
    node start;
    start.v = sv;
    start.parent = NULL;
    start.heaviest = 0;

    // BFS to find the cycle from target to itsself
    // Keep the largest weight on it.

    q.push(&start);
    assert(q.size() == 1);

    bool cycle = false;
    int heaviest = 0;
    while (!cycle) {
      node* cur = q.top();
      q.pop();

      Vertex from = cur->v;

      vector<int>& ns = neighbors[from];
      for (auto it = ns.begin(); it < ns.end(); ++it) {
        Vertex to = *it;

        if (to == target(adding, g)) {
          cycle = true;
          heaviest = max(cur->heaviest, weight[from][to]);
          break;
        }

        if (from == to) { continue; }
        if (cur->parent != NULL && to == cur->parent->v) { continue; } // Don't go back without a loop.

        node* n = new node;
        n->v = to;
        n->parent = cur;
        n->heaviest = max(cur->heaviest, weight[from][to]);
        q.push(n);
      }
    }

    w -= heaviest;

    minweight = std::min(minweight, w);
  }

  return minweight;
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
