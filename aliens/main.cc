#include <iostream>
#include <vector>
#include <queue>

constexpr int ALIEN_MEMORY = 3;

enum color { WHITE, GREY, BLACK };
enum species { HUMAN, ALIEN };

struct interval {
  int first;
  int last;
};

void print_intervals(std::vector<interval>& ais) {
  std::cout << "alien map:" << std::endl;;

  int n = ais.size();
  for (int i = 0; i < n; ++i) {
    std::cout << i << ": ";
    if (ais[i].first == -1 && ais[i].last == -1) {
      std::cout << std::endl;
      continue;
    }
    for (int j = ais[i].first; j <= ais[i].last; ++j) {
      std::cout << j << " ";
    }
    std::cout << std::endl;
  }
}

void print_reverse_map(std::vector<std::vector<int>>& reverse_map) {
  std::cout << "human map:" << std::endl;

  int n = reverse_map.size();
  for (int i = 0; i < n; ++i) {
    std::cout << i << ": ";
    int m = reverse_map[i].size();
    for (int j = 0; j < m; ++j) {
      std::cout << reverse_map[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

struct vertex {
  int number;
  species sp;
  int hops;
};

int solve (int humans, int aliens, std::vector<interval>& ais) {
  // Build reverse map
  std::vector<std::vector<int>> reverse_map(humans);
  for (int a = 0; a < aliens; ++a) {
    interval i = ais[a];
    if (i.first == -1 && i.last == -1) {
      for (int h = 0; h < humans; ++h) {
        reverse_map[h].push_back(a);
      }
    } else {
      for (int h = 0; h < i.first; ++h) {
        reverse_map[h].push_back(a);
      }
      for (int h = i.last + 1; h < humans; ++h) {
        reverse_map[h].push_back(a);
      }
    }
  }

  int counter = 0;

  for (int alien = 0; alien < aliens; ++alien) {

    // visited structures
    color visited_humans[humans];
    for (int h = 0; h < humans; ++h) {
      visited_humans[h] = WHITE;
    }
    color visited_aliens[aliens];
    for (int a = 0; a < aliens; ++a) {
      visited_aliens[a] = WHITE;
    }

    std::queue<vertex> q;

    vertex start;
    start.number = alien;
    start.sp = ALIEN;
    start.hops = 0;
    q.push(start);

    while (!q.empty()) {
      vertex cur = q.front();
      q.pop();

      //std::cout << "handling: " << std::endl;
      //std::cout << cur.number << std::endl;
      //std::cout << cur.sp << std::endl;
      //std::cout << cur.hops << std::endl;

      // In BFS, we can stop as soon as one is too large
      if (cur.hops > ALIEN_MEMORY) { break; }

      if (cur.sp == ALIEN) {
        int a = cur.number;
        if (!(ais[a].first == -1 && ais[a].last == -1)) { 
          for (int h = ais[a].first; h <= ais[a].last; ++h) {
            if (visited_humans[h] != WHITE) { continue; }
            vertex next;
            next.number = h;
            next.sp = HUMAN;
            next.hops = cur.hops + 1;
            q.push(next);

            visited_humans[h] = GREY;
          }
        }
        visited_aliens[a] = BLACK;
      } else if (cur.sp == HUMAN) {
        int h = cur.number;
        for (auto it = reverse_map[h].begin(); it < reverse_map[h].end(); ++it) {
          int a = *it;
          if (visited_aliens[a] != WHITE) { continue; }
          vertex next;
          next.number = *it;
          next.sp = ALIEN;
          next.hops = cur.hops + 1;
          q.push(next);

          visited_aliens[a] = GREY;
        }
        visited_humans[h] = BLACK;
      } else {
        exit(1); // Cannot happen, right?
      }
    }

    bool allSuperior = true;
    for (int h = 0; h < humans; ++h) {
      if (visited_humans[h] != BLACK) {
        allSuperior = false;
        break;
      }
    }
    if (!allSuperior) { continue; }
    for (int a = 0; a < aliens; ++a) {
      if (a == alien) { continue; } // Doesn't have to be superior to itself.
      if (visited_aliens[a] != BLACK) {
        allSuperior = false;
        break;
      }
    }
    if (allSuperior) { counter++; }
  }

  //print_intervals(ais);
  //print_reverse_map(reverse_map);

  return counter;
}



int main() {
  int cases;
  std::cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int n, m;
    std::cin >> n; // aliens
    std::cin >> m; // humans

    std::vector<interval> intervals;
    for (int i = 0; i < n; ++i) {
      interval in;
      std::cin >> in.first;
      std::cin >> in.last;
      --in.first; // zero indexed 
      --in.last; // zero indexed
      intervals.push_back(in);
    }
    std::cout << solve(m, n, intervals) << std::endl;
  }
}
