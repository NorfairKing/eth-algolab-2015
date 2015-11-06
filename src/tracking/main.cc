#include <iostream>
#include <vector>

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, 
        vecS, 
        undirectedS, 
        no_property,
        property<edge_weight_t, int>> 
        Graph;

typedef graph_traits<Graph>::edge_descriptor      Edge;
typedef graph_traits<Graph>::vertex_descriptor    Vertex;
typedef graph_traits<Graph>::edge_iterator        EdgeIt;
typedef property_map<Graph, edge_weight_t>::type  WeightMap;

struct road {
  int from;
  int to;
  int length;
  bool river;
};

static int k;

int nr(int v, int rivers) {
  return v * (k + 1) + rivers;
}

void add(
    Graph& g, WeightMap& wm
    , int len
    , int from, int from_rivers
    , int to, int to_rivers) {
  int f = nr(from, from_rivers);
  int t = nr(to, to_rivers);

  Edge e; bool s;
  tie(e, s) = add_edge(f, t, g);
  assert(s);
  wm[e] = len;
}

int solve(int n, int m, int kk, int x, int y, vector<road>& roads) {
  k = kk;
  Graph g;
  WeightMap wm = get(edge_weight, g);

  for (int i = 0; i < m; ++i) {
    int from  = roads[i].from;
    int to    = roads[i].to;
    int len   = roads[i].length;
    if (roads[i].river) {
      for (int j = 0; j < k; ++j) {
        add(g, wm, len, from, j    , to, j + 1);
        add(g, wm, len, from, j + 1, to, j); // Don't forget this one!
      }
      add(g, wm, len, from, k, to, k);
    } else {
      for (int j = 0; j <= k; ++j) {
        add(g, wm, len, from, j, to, j);
      }
    }
  }

  int from = nr(x, 0);
  int to = nr(y, k);
  
  int num_vertex = num_vertices(g);
  assert(num_vertex == (n * (k + 1)));

  vector<int> dist(num_vertex);
  dijkstra_shortest_paths(g, from,
      distance_map(make_iterator_property_map(dist.begin(), get(vertex_index, g))));

  return dist[to];
}

int main () {
  int cases;
  cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int n, m, k, x, y;
    cin >> n >> m >> k >> x >> y;

    vector<road> roads(m);
    for (int i = 0; i < m; ++i) {
      int a, b, c, d;
      cin >> a >> b >> c >> d;

      road ri;
      ri.from = a;
      ri.to = b;
      ri.length = c;
      ri.river = d;
      roads[i] = ri;
    }

    cout << solve(n, m, k, x, y, roads) << endl;
  }
}
