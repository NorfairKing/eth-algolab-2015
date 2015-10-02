#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <boost/optional.hpp>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Segment_2 S;
typedef K::Ray_2 R;
typedef K::Point_2 P;

double floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void replace_closest(R& ray ,boost::optional<P>& closest, const P& point) {
  if(closest) {
    if (CGAL::squared_distance(ray.source(), point)
        < CGAL::squared_distance(ray.source(), *closest)) {
      closest = point;
    }
  } else {
    closest = point;
  }
}

boost::optional<P> solve(R ray, std::vector<S>& obstacles) {
  boost::optional<P> closest;
  for (auto o: obstacles) {
    bool intersect = CGAL::do_intersect(ray, o);
    if (intersect) {
      auto i = CGAL::intersection(ray, o);
      if (const P* ip = boost::get<P>(&*i)) {
        replace_closest(ray, closest, *ip);
      } else if (const S* is = boost::get<S>(&*i)) {
        replace_closest(ray, closest, (*is).source());
        replace_closest(ray, closest, (*is).target());
      } else { exit(1); } // Cannot happen?
    }
  }
  return closest;
}

int main() {
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  while (true) {
    int n;
    std::cin >> n;
    if (n == 0) { break; }

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

    auto solution = solve(ray, obstacles);
    if (solution) {
      P& point = solution.get();
      long px = floor_to_double(point.x());
      long py = floor_to_double(point.y());
      std::cout << px << " " << py << std::endl;
    } else {
      std::cout << "no" << std::endl;
    }
  }
}
