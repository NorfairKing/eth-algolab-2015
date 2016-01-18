#include <iostream>
#include <vector>
#include <tuple>
#include <stack>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>


using namespace std;
using namespace boost;


typedef adjacency_list<vecS, vecS, undirectedS> Graph;
typedef graph_traits<Graph>::edge_descriptor    Edge;
typedef graph_traits<Graph>::vertex_descriptor  Vertex;
typedef graph_traits<Graph>::out_edge_iterator  OutEdgeIt;

struct lnk {
  int station;
  int satellite;
};

pair<vector<int>, vector<int>> solve(
      int g, int s, int l
    , vector<lnk>& lnks) {
  int t = g + s;
  Graph graph(t);

  for (int i = 0; i < l; ++i) {
    lnk& li = lnks[i];
    Vertex from =     li.station;
    Vertex to   = g + li.satellite;
    Edge e; bool success;
    tie(e, success) = add_edge(from, to, graph);
    assert(success);
  }

  // In a bipartite graph like this, the number of edges in the maximum
  // matching is equal to the number of vertices in a minimum vertex matching.
  // find maximum matching
  vector<Vertex> mate(t);
  checked_edmonds_maximum_cardinality_matching(graph, &mate[0]);


  // L: Left vertices in biparte graph
  // R: Right vertices in bipartite graph
  
  stack<int> q;
  vector<bool> visited(t, false);

  // step 1: mark all unmatched vertices in L as visited.
  for (int i = 0; i < g; ++i) {
    if (mate[i] == graph_traits<Graph>::null_vertex()) {
      q.push(i);
    } 
	}

  // Run DFS
	while (!q.empty()) {
		int from = q.top();
		q.pop();

		visited[from] = true;

		OutEdgeIt ebeg, eend;
		for (tie(ebeg, eend) = out_edges(from, graph); ebeg != eend; ++ebeg) {
			int to = target(*ebeg, graph);
      if (visited[to]) { continue; }

      int m = mate[from];

      // Go left to right along edges outside the matching
      // Go right to left along edges in the matching
      bool follow = (from < to && m != to)
                 || (to < from && m == to);
      if (follow) { q.push(to); }
		}
	}

  vector<int> stations;
  vector<int> satellites;

  // All unvisited in L
  for (int i = 0; i < g; ++i) {
    if (visited[i]) { continue; }
    stations.push_back(i);
  }

  // All visited in R
  for (int i = g; i < t; ++i) {
    if (!visited[i]) { continue; }
    satellites.push_back(i - g);
  }

  return make_pair(stations, satellites);
}

int main() {
  int cases;
  cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int g, s, l;
    cin >> g >> s >> l;

    vector<lnk> ls(l);
    for (int i = 0; i < l; ++i) {
      cin >> ls[i].station;
      cin >> ls[i].satellite;
    }

    pair<vector<int>, vector<int>> solution = solve(g, s, l, ls);
    
    vector<int>& sts = solution.first;
    vector<int>& sas = solution.second;
    int n = sts.size();
    int m = sas.size();
    cout << n << " " << m << endl;
		// Spaces at the end of the line aren't a problem.
    for (int i = 0; i < n; ++i) {
      cout << sts[i] << " ";
    }
    for (int i = 0; i < m; ++i) {
      cout << sas[i] << " ";
    }
    cout << endl;
  }
}
