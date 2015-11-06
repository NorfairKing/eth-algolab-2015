#include <iostream>
#include <vector>
#include <queue>
#include <climits>

int constexpr P = 2;

struct edge {
  int from;
  int to;
};

struct vertex {
  int num;
  int distance; 
};

std::vector<std::vector<int>> precompute_neigbors(
    int nr_vertices, std::vector<edge>& edges) {
  std::vector<std::vector<int>> neigbors(nr_vertices);
  int nr_edges = edges.size();
  for (int i = 0; i < nr_edges; ++i) {
    neigbors[edges[i].from].push_back(edges[i].to);
  }
  return neigbors;
}

int solve(int nr_edges, int nr_vertices, int from, int to, std::vector<edge>& edges) {
  std::vector<std::vector<int>> neigbors = precompute_neigbors(nr_vertices, edges);

  std::vector<int> visited(nr_vertices, 0);
  
  vertex start;
  start.distance = 0;
  start.num = from;

  std::queue<vertex> q;
  q.push(start);
  while (! q.empty()) {
    vertex& cur = q.front();
    q.pop();

    int c = cur.num;
    int vis = visited[c];

    if (c == to && vis == (P - 1)) { return cur.distance; }
    if (vis >= P) { continue; }

    std::vector<int>& ns = neigbors[c];
    for (auto it = ns.begin(); it < ns.end(); ++it) {
      vertex n;
      n.distance = cur.distance + 1;
      n.num = *it;
      q.push(n);
    }

    visited[c]++;
  }

  return -1;
}

int main () {
  std::ios_base::sync_with_stdio(false);
  int cases;
  std::cin >> cases;

  for (int c = 0; c < cases; ++c) {
    int m, n;
    std::cin >> n >> m;
    int s, t;
    std::cin >> s >> t;
    s--; t--; // zero indexed.

    std::vector<edge> edges;
    for (int i = 0; i < m; ++i) {
      edge e;
      
      std::cin >> e.from;
      std::cin >> e.to;
      e.from--; // zero indexed
      e.to--; // zero indexed

      edges.push_back(e);
    }

    int result = solve(m, n, s, t, edges);

    if (result == -1) {
      std::cout << "no" << std::endl;
    } else {
      std::cout << result << std::endl;
    }
  }
}
