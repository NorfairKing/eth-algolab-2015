#include <iostream>
#include <vector>
#include <map>
#include <set>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

bool solve(int nr_students, vector<set<int>> students, int nr_chars, int min_chars) {
  Graph g(nr_students);
  for (int i = 0; i < nr_students; ++i) {
    for (int j = i + 1; j < nr_students; ++j) {
      Edge edge; bool success;
      vector<int> intersection(nr_chars);
      auto it = set_intersection(students[i].begin(), students[i].end()
                               , students[j].begin(), students[j].end()
                               , intersection.begin());
      int int_size = it - intersection.begin();
      if (int_size <= min_chars) { continue; }
      tie(edge, success) = add_edge(i, j, g);
    }
  }

  vector<Vertex> mate(nr_students);
  checked_edmonds_maximum_cardinality_matching(g, &mate[0]);
  
  int size = matching_size(g, &mate[0]);
  return size != (nr_students / 2);
}

int main() {
  int cases;
  cin >> cases;
  for (int tcase = 0; tcase < cases; ++tcase) {
    int n, c, f;
    cin >> n >> c >> f;

    vector<set<int>> students(n);
    int wordcount = 0; // Zero indexed words
    map<string, int> strmap;
    for (int i = 0; i < n; ++i) {
      set<int> student;
      for (int j = 0; j < c; ++j) {
        string s;
        cin >> s;

        if (strmap.find(s) == strmap.end()) { // Doesn't contain s yet
          strmap.insert(make_pair(s, wordcount));
          ++wordcount;
        }
        student.insert(strmap[s]);
      }
      students[i] = student;
    }
    if (solve(n, students, c, f)) {
      cout << "optimal" << endl;
    } else {
      cout << "not optimal" << endl;
    }
  }
}
