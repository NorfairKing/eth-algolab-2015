#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef K::Point_2 P;

long ceil_to_long(const K::FT& x) {
  long a = std::floor(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a - 1 >= x) a -= 1;
  return a;
}

long solve(std::vector<P> homes) {
  Min_circle mc2(homes.begin(), homes.end(), true); // Randomize input.
  CGAL::set_pretty_mode(std::cout);
  K::FT radius = sqrt(mc2.circle().squared_radius());
  return ceil_to_long(radius);
}

int main() {
  while (true) {
    int n;
    std::cin >> n;
    if (n == 0) { break; }

    std::vector<P> homes;
    for (int i = 0; i < n; ++i) {
      long x, y;
      std::cin >> x >> y;
      P h(x, y);
      homes.push_back(h);
    }

    std::cout << solve(homes) << std::endl;
  }
}
