#include <iostream>
#include <vector>

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>  // tuples::ignore

using namespace std;
using namespace boost;

typedef adjacency_list<
        vecS,
        vecS,
        undirectedS,
        no_property,
        property<edge_weight_t, int>>             Graph;
typedef graph_traits<Graph>::edge_descriptor      Edge;
typedef graph_traits<Graph>::edge_iterator        EdgeIt;
typedef property_map<Graph, edge_weight_t>::type  WeightMap;

pair<int, int> solve(Graph& graph) {
  WeightMap wm = get(edge_weight, graph);

  vector<Edge> mst;
  kruskal_minimum_spanning_tree(graph, back_inserter(mst));
  vector<Edge>::iterator ebeg, eend = mst.end();

  int weight_sum = 0;
  for (ebeg = mst.begin(); ebeg != eend; ++ebeg) {
    weight_sum += wm[*ebeg];
  }

  int vertices = num_vertices(graph);
  vector<int> dist(vertices);
  vector<int> pred(vertices);
  dijkstra_shortest_paths(graph, 0,
    predecessor_map(make_iterator_property_map(pred.begin(),
            get(vertex_index, graph))).
    distance_map(make_iterator_property_map(dist.begin(),
            get(vertex_index, graph))));

  int max_dist = 0;
  for (int i = 0; i < vertices; ++i) {
    max_dist = std::max(max_dist, dist[i]);
  }
  return make_pair(weight_sum, max_dist);
}

int main() {
  ios_base::sync_with_stdio(false);
  int cases;
  cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int n,m;
    cin >> n >> m;

    Graph g(n);
    WeightMap wm = get(edge_weight, g);

    for (int i = 0; i < m; ++i) {
      int start, end, weight;
      cin >> start >> end >> weight;

      Edge e;
      tie(e, tuples::ignore) = add_edge(start, end, g);
      wm[e] = weight;
    }
    pair<int, int> solution = solve(g);
    cout << solution.first << " " << solution.second << endl;
  }
}
