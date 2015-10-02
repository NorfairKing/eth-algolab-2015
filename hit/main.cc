#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Segment_2 S;
typedef K::Ray_2 R;
typedef K::Point_2 P;

bool solve(R ray, std::vector<S>& obstacles) {
  for (auto o: obstacles) {
    bool intersect = CGAL::do_intersect(ray, o);
    if (intersect) {
      return true;
    }
  }
  return false;
}

int main() {
  while(true) {
    int n;
    std::cin >> n;
    if(n == 0) { break; }
    long x, y, a, b;
    std::cin >> x >> y >> a >> b;
    P p(x, y), q(a, b);
    R ray(p, q);

    std::vector<S> obstacles;

    for (int i = 0; i < n; ++i) {
      long fx, fy, tx, ty;
      std::cin >> fx >> fy >> tx >> ty;
      P f(fx, fy), t(tx, ty);
      S o(f, t);
      obstacles.push_back(o);
    }

    if (solve(ray, obstacles)) {
      std::cout << "yes" << std::endl;
    } else {
      std::cout << "no" << std::endl;
    }
  }
}
