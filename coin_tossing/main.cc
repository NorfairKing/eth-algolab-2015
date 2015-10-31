#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS>        Traits;
typedef adjacency_list<
    vecS
  , vecS
  , directedS
  , no_property
  , property<edge_capacity_t, long,
      property<edge_residual_capacity_t, long,
        property<edge_reverse_t, Traits::edge_descriptor>>>>  Graph;
typedef property_map<Graph, edge_capacity_t>::type            EdgeCapacityMap;
typedef property_map<Graph, edge_residual_capacity_t>::type   ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type             ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor                Vertex;
typedef graph_traits<Graph>::edge_descriptor                  Edge;
typedef graph_traits<Graph>::edge_iterator                    EdgeIt; // Iterator

struct EdgeAdder {
  EdgeAdder(Graph & G, EdgeCapacityMap &capacity, ReverseEdgeMap &rev_edge)
    : G(G), capacity(capacity), rev_edge(rev_edge) {}

  void addEdge(int u, int v, long c) {
    Edge e, reverseE;
    tie(e, tuples::ignore) = add_edge(u, v, G);
    tie(reverseE, tuples::ignore) = add_edge(v, u, G);
    capacity[e] = c;
    capacity[reverseE] = 0;
    rev_edge[e] = reverseE;
    rev_edge[reverseE] = e;
  }
  Graph &G;
  EdgeCapacityMap &capacity;
  ReverseEdgeMap  &rev_edge;
};


enum outcome { UK, P1, P2 };
struct game {
  int p1;
  int p2;
  outcome o;
};

bool solve (int n, int m, vector<game> games, vector<int> scores) {
  // Can't have more points than games
  int sum = 0;
  for (int i = 0; i < n; ++i) {
    sum += scores[i];
  }
  if (sum != m) { return false; }

  // Can't get less points than you already have.
  vector<int> left(scores);
  for (int i = 0; i < m; ++i) {
    game& gg = games[i];

    switch (gg.o) {
      case UK:
        continue;
      case P1:
        --left[gg.p1];
        break;
      case P2:
        --left[gg.p2];
        break;
    }
  }
  for (int i = 0; i < n; ++i) {
    if (left[i] < 0) { return false; }
  }

  // Now this is a flow problem!
  //
  // For every game, have an edge on the left and an edge from the source with capacity one.
  // This models that at most one person can win the game
  //
  // For every player, have a node on the right that receives edges from the games it's in with capacity 1
  // 
  // For every player, have a link to the sink with the score required according to the leaderboard.
  // Solve the maxflow problem.
  // If the flow is equal to the sum of the leaderboard, it's possible.
  Graph g;
  EdgeCapacityMap capacity = get(edge_capacity, g);
  ReverseEdgeMap rev_edge = get(edge_reverse, g);
  EdgeAdder ea(g,capacity, rev_edge);

  vector<Vertex> pnode(n);
  vector<Vertex> gnode(m);
  for (int i = 0; i < n; ++i) {
    Vertex pp = add_vertex(g);
    pnode[i] = pp;
  }
  for (int i = 0; i < m; ++i) {
    Vertex gg = add_vertex(g);
    gnode[i] = gg;
  }

  Vertex src = add_vertex(g);
  Vertex snk = add_vertex(g);

  // From source to each game
  for (int i = 0; i < m; ++i) {
    ea.addEdge(src, gnode[i], 1);
  }

  // From games to players
  for (int i = 0; i < m; ++i) {
    game& gg = games[i];
    Vertex gn = gnode[i];
    Vertex p1 = pnode[gg.p1];
    Vertex p2 = pnode[gg.p2];

    switch (gg.o) {
      case UK:
        ea.addEdge(gn, p1, 1);
        ea.addEdge(gn, p2, 1);
        break;
      case P1:
        ea.addEdge(gn, p1, 1);
        break;
      case P2:
        ea.addEdge(gn, p2, 1);
        break;
    }
  }

  // From players to sink
  for (int i = 0; i < n; ++i) {
    Vertex p = pnode[i];
    ea.addEdge(p, snk, scores[i]);
  }

  int flow = push_relabel_max_flow(g, src, snk);


  return sum == flow;
}

int main() {
  int cases;
  cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int n, m;
    cin >> n >> m;

    vector<game> gs;
    for (int i = 0; i < m; ++i) {
      int a, b, c;
      cin >> a >> b >> c;
      game g;
      g.p1 = a;
      g.p2 = b;
      g.o = (outcome) c;
      gs.push_back(g);

    }
    vector<int> ss;
    for (int i = 0; i < n; ++i) {
      int s;
      cin >> s;
      ss.push_back(s);
    }

    if (solve(n, m, gs, ss)) {
      cout << "yes" << endl;
    } else {
      cout << "no" << endl;
    }
    // cout << endl; // debugging
  }
}
