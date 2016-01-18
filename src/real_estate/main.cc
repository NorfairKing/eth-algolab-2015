#include <iostream>
#include <vector>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS>        Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
        property<edge_capacity_t, long,
          property<edge_residual_capacity_t, long,
            property<edge_reverse_t, Traits::edge_descriptor,
              property <edge_weight_t, long>>>>>            Graph;
typedef property_map<Graph, edge_capacity_t>::type          EdgeCapacityMap;
typedef property_map<Graph, edge_weight_t >::type           EdgeWeightMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type           ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor              Vertex;
typedef graph_traits<Graph>::edge_descriptor                Edge;
typedef graph_traits<Graph>::out_edge_iterator              OutEdgeIt;

struct EdgeAdder {
  EdgeAdder(Graph & G, EdgeCapacityMap &capacity, EdgeWeightMap &weight, ReverseEdgeMap &rev_edge)
    : G(G), capacity(capacity), weight(weight), rev_edge(rev_edge) {}

  void addEdge(int u, int v, long c, long w) {
    Edge e, reverseE;
    tie(e, tuples::ignore) = add_edge(u, v, G);
    tie(reverseE, tuples::ignore) = add_edge(v, u, G);
    capacity[e] = c;
    weight[e] = w;
    capacity[reverseE] = 0;
    weight[reverseE] = -w;
    rev_edge[e] = reverseE;
    rev_edge[reverseE] = e;
  }
  Graph &G;
  EdgeCapacityMap &capacity;
  EdgeWeightMap &weight;
  ReverseEdgeMap  &rev_edge;
};


pair<int, int> solve(int n, int m, int s,
                     vector<int>& limits,
                     vector<int>& states,
                     vector<vector<int>>& bids) {

  int t = n + m;

  // Find out what the maximum bid was, this will help with complexity later.
  int maxbid = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      maxbid = max(maxbid, bids[i][j]);
    }
  }

  Graph g(t);
  EdgeCapacityMap capacity = get(edge_capacity, g);
  EdgeWeightMap weight = get(edge_weight, g);
  ReverseEdgeMap rev_edge = get(edge_reverse, g);
  // ResCapacityMap res_capacity = get(edge_residual_capacity, g);
  EdgeAdder ea(g, capacity, weight, rev_edge);

  vector<Vertex> ss(s);
  for (int k = 0; k < s; ++k) {
    ss[k] = add_vertex(g);
  }

  Vertex src = add_vertex(g);
  Vertex snk = add_vertex(g);

  // source to buyer: buyer can only buy 1 house
  for (int i = 0; i < n; ++i) {
    ea.addEdge(src, i, 1, 0); // cap 1 cost 0
  }

  // buyer to house
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      // We add maxbid to get something positive
      ea.addEdge(i, j + n, 1, maxbid - bids[i][j]); // cap 1 cost minus the bid
    }
  }

  // house to state: every house can only be bought once
  for (int j = 0; j < m; ++j) {
    ea.addEdge(j + n, ss[states[j]], 1, 0); // cap 1 cost 0
  }

  // state to sink: maximum number of houses sold in state
  for (int k = 0; k < s; ++k) {
    ea.addEdge(ss[k], snk, limits[k], 0); // cap limits[j] cost 0
  }

  // maximum flow = most houses sold (houses can only make profit)
  int flow = push_relabel_max_flow(g, src, snk);
  successive_shortest_path_nonnegative_weights(g, src, snk);
  int cost = find_flow_cost(g);
  // For every unit of flow, one time maxbid will be added to the cost.
  // minimum cost = negative the maximum profit plus flow * maxbid


  return make_pair(flow, flow * maxbid - cost);
}

int main() {
  int cases;
  cin >> cases;

  for (int c = 0; c < cases; ++c) {
    int n, m, s;
    cin >> n >> m >> s;

    vector<int> ls(s);
    for (int i = 0; i < s; ++i) {
      cin >> ls[i];
    }

    vector<int> ss(m);
    for (int i = 0; i < m; ++i) {
      cin >> ss[i];
      ss[i]--; //zero indexed
    }

    vector<vector<int>> bs(n, vector<int>(m));
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        cin >> bs[i][j];
      }
    }

    pair<int, int> sol = solve(n, m, s, ls, ss, bs);
    cout << sol.first << " " << sol.second << endl;
  }
}
