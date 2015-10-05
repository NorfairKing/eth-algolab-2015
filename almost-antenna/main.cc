#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <boost/optional.hpp>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef K::Point_2 P;

double ceil_to_double(const K::FT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a - 1 >= x) a -= 1;
  return a;
}

double solve(std::vector<P> homes) {
  boost::optional<K::FT> smallest;

  int n = homes.size();
  for (int i = 0; i < n; ++i) {
    // swap i with last = n-1
    P tmp = homes[i];
    homes[i] = homes[n - 1];
    homes[n - 1] = tmp;

    // Get minimum circle for all except the ith one
    Min_circle mc(homes.begin(), homes.end() - 1, true); // Randomize input.
    if (smallest) {
      if (mc.circle().squared_radius() < smallest.get()) {
          smallest = mc.circle().squared_radius();
      }
    } else {
      smallest = mc.circle().squared_radius();
    }
  }

  if (smallest) {
    return ceil_to_double(sqrt(smallest.get()));
  } else {
    return 0;
  }
}

int main() {
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);

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
