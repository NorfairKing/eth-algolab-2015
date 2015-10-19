#include <iostream>
#include <algorithm>
#include <tuple>
#include <vector>
#include <list>
#include <iterator>

#include <boost/config.hpp>
#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/adjacency_list.hpp>


using namespace std;
using namespace boost;


struct edge_component_t
{
  enum
  { num = 555 };
  typedef edge_property_tag kind;
}
edge_component;
typedef adjacency_list<
  vecS,
  vecS,
  undirectedS,
  no_property,
  property<edge_component_t, size_t> >              Graph;
typedef graph_traits<Graph>::vertex_descriptor      Vertex;
typedef graph_traits<Graph>::edge_descriptor        Edge;
typedef graph_traits<Graph>::edge_iterator          EdgeIt;
typedef property_map<Graph, edge_component_t>::type CompMap;

struct bridge {
  int from;
  int to;
};

bool operator < (const bridge& x, const bridge& y) {
  if (x.from == y.from) {
    return x.to < y.to;
  } else {
    return x.from < y.from;
  }
}

vector<bridge> solve(int n, int m, vector<bridge> bs) {
  Graph g(n);
  for (int i = 0; i < m; ++i) {
    add_edge(bs[i].from, bs[i].to, g);
  }

  CompMap component = get(edge_component, g);
  size_t num_comps = biconnected_components(g, component);

  vector<vector<bridge>> comps(num_comps);
  EdgeIt ei, ei_end;
  for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
    bridge b;
    b.from = source(*ei, g) ;
    b.to = target(*ei, g);
    comps[component[*ei]].push_back(b);
  }

  vector<bridge> solution;
  for (size_t i = 0; i < num_comps; ++i) {
    if (comps[i].size() == 1) {
      solution.push_back(comps[i][0]);
    }
  }
  return solution;
}

int main() {
  ios_base::sync_with_stdio(false);
  int cases;
  cin >> cases;

  for (int c = 0; c < cases; ++c) {
    int m, n;
    cin >> n >> m;

    vector<bridge> bs;
    for (int i = 0; i < m; ++i) {
      bridge b;
      cin >> b.from >> b.to;
      bs.push_back(b);
    }

    vector<bridge> solution = solve(n, m, bs);
    int s = solution.size();

    for (int i = 0; i < s; ++i) {
      int from = solution[i].from;
      int to   = solution[i].to;
      solution[i].from = min(from, to);
      solution[i].to   = max(from, to);
    }

    sort(solution.begin(), solution.end());

    cout << s << endl;
    for (int i = 0; i < s; ++i) {
      cout << min(solution[i].from, solution[i].to) << " " << max(solution[i].from, solution[i].to) << endl;
    }
  }
}
