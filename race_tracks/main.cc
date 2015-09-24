#include <iostream>
#include <vector>
#include <queue>

constexpr int maxvelocity = 3;
constexpr int minvelocity = -3;

enum color { WHITE, GRAY, BLACK };

struct velocity {
  int x;
  int y;

  bool operator== (const velocity& that) {
    return (this->x == that.x && this->y == that.y);
  }

  std::vector<velocity> neighbors() {
    std::vector<velocity> neighbors;
    for (int dx = -1; dx <= 1; ++dx) {
      for (int dy = -1; dy <= 1; ++dy) {
        if (dx == 0 && dy == 0) { continue; } // There would be no use.
        int newx = x + dx;
        if (!(newx <= maxvelocity && newx >= minvelocity)) { continue; }
        int newy = y + dy;
        if (!(newy <= maxvelocity && newy >= minvelocity)) { continue; }
        velocity v;
        v.x = newx;
        v.y = newy;
        neighbors.push_back(v);
      }
    }
    return neighbors;
  }
};
                                    
std::ostream& operator<< (std::ostream &out, const velocity &vel) {
  out << "Vel: (" << vel.x << ", " << vel.y << ")" << std::endl;
  return out;
}

struct position {
  int x;
  int y;

  position addvelocity(const velocity& vel) {
    position p;
    p.x = x + vel.x;
    p.y = y + vel.y;
    return p;
  }

  bool operator== (const position& that) {
    return (this->x == that.x && this->y == that.y);
  }

  bool isWithinField(const int xlen, const int ylen) {
    return (x >= 0 && x < xlen && y >= 0 && y < ylen);
  }
};


std::ostream& operator<< (std::ostream& out, position& pos)
{
  out << "Pos: (" << pos.x << ", " << pos.y << ")" << std::endl;
  return out;
}

class Vertex {
  public:
    position pos;
    velocity vel;
    int hops;
    Vertex(const position& pos);
    Vertex(const position& pos, const velocity& vel, int hops);
    friend std::ostream& operator<< (std::ostream& out, const Vertex& v);
    bool operator== (const Vertex& that) {
      return (this->pos == that.pos && this->vel == that.vel);
    }
};

Vertex::Vertex(const position& pos) {
  this->pos = pos;
  velocity vel;
  vel.x = 0;
  vel.y = 0;
  this->vel = vel;
  this->hops = 0;
}

Vertex::Vertex(const position& pos, const velocity& vel, int hops) {
  this->pos = pos;
  this->vel = vel;
  this->hops = hops;
}

std::ostream& operator<< (std::ostream& out, const Vertex& v) {
  out << v.pos;
  out << v.vel;
  out << "Hops: " << v.hops << std::endl;
  return out;
}

struct obstacle {
  int x1;
  int x2;
  int y1;
  int y2;
};

bool** precompute_free(int xlen, int ylen, const std::vector<obstacle>& obstacles) {
  bool** free;
  free = new bool *[xlen];
  for(int x = 0; x < xlen; ++x) {
    free[x] = new bool[ylen];
    for (int y = 0; y < ylen; ++y) {
      free[x][y] = true;
    }
  }
  for (auto it = obstacles.begin(); it < obstacles.end(); ++it) {
    for (int x = it->x1; x <= it->x2; ++x) {
      for (int y = it->y1; y <= it->y2; ++y) {
        free[x][y] = false;
      }
    }
  }
  return free;
}

int solve(int xlen, int ylen, const position& startpos, const position& endpos, const std::vector<obstacle>& obstacles) {
  bool** free = precompute_free(xlen, ylen, obstacles);

  int velocities = maxvelocity - minvelocity + 1;
  int shift = abs(minvelocity);
  //std::cout << velocities << std::endl;

  color visited[xlen][ylen][velocities][velocities];
  //std::cout << "created array" << std::endl;

  // Assign values to the elements
  for(int px = 0; px < xlen; ++px) {
    for(int py = 0; py < ylen; ++py) {
      for(int vx = 0; vx < velocities; ++vx) {
        for(int vy = 0; vy < velocities; ++vy) {
          visited[px][py][vx][vy] = WHITE;
        }
      }
    }
  }
  //std::cout << xlen * ylen * velocities * velocities << std::endl;


  //for (int x = 0; x < xlen; ++x) {
  //  for (int y = 0; y < ylen; ++y) {
  //    std::cout << free[x][y];
  //  }
  //  std::cout << std::endl;
  //}
  if (! free[endpos.x][endpos.y]) { return -1; }

  //std::cout << xlen << " x " << ylen << std::endl;
  //std::cout << "Going to: " << endpos;

  std::queue<Vertex> q; 
  //std::unordered_set<Vertex> visited;

  Vertex start(startpos);

  //velocity direction;
  //direction.x = sign(endpos.x - startpos.x);
  //direction.y = sign(endpos.y - startpos.y);
  //std::cout << "direction: " << direction;

  q.push(start);
  while (!q.empty()) {
    Vertex cur = q.front();
    q.pop();

    //std::cout << "handling: " << std::endl;
    //std::cout << cur;
    
    position cp = cur.pos;
    if (cp == endpos) {
      return cur.hops;
    }

    velocity cv = cur.vel;
    std::vector<velocity> neighbors = cv.neighbors();


    for (auto it = neighbors.begin(); it < neighbors.end(); ++it) {
      //std::cout << "neighbor: " << *it;

      velocity v = *it;
      position p = cp.addvelocity(v);

      if (! p.isWithinField(xlen, ylen)) { continue; }
      if (! free[p.x][p.y]) { continue; }
      if (visited[p.x][p.y][v.x + shift][v.y + shift] != WHITE) { continue; }

      Vertex next = Vertex(p, v, cur.hops + 1);

      //std::cout << "adding: " << std::endl;
      //std::cout << next;
      //std::cout << std::endl;

      q.push(next);
      visited[p.x][p.y][v.x + shift][v.y + shift] = GRAY;
    }

    //std::cout << "done with node." << std::endl;
    //std::cout << std::endl;
    //std::cout << std::endl;
    visited[cur.pos.x][cur.pos.y][cur.vel.x + shift][cur.vel.y + shift] = BLACK;
  }

  //std::cout << visited[0][0][0][0] << std::endl;
  return -1;
}

int main() {
  int cases;
  std::cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int xlen, ylen;
    std::cin >> xlen;
    std::cin >> ylen;

    position startpos;
    position endpos;
    std::cin >> startpos.x;
    std::cin >> startpos.y;
    std::cin >> endpos.x;
    std::cin >> endpos.y;

    int p;
    std::cin >> p;
    std::vector<obstacle> obstacles;
    for (int i = 0; i < p; ++i) {
      obstacle o;
      std::cin >> o.x1;
      std::cin >> o.y1;
      std::cin >> o.x2;
      std::cin >> o.y2;
      obstacles.push_back(o);
    }
    
    int solution = solve(xlen, ylen, startpos, endpos, obstacles);
    if (solution >= 0) {
      std::cout << "Optimal solution takes " << solution << " hops." << std::endl;
    } else {
      std::cout << "No solution." << std::endl;
    }
  }
}



