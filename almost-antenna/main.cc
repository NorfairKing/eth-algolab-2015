#include <iostream>
#include <vector>
#include <limits>

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef K::Point_2 P;

double ceil_to_double(const K::FT& x) {
  double a = ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a - 1 >= x) a -= 1;
  return a;
}

double solve(vector<P> homes) {
  boost::optional<K::FT> smallest;
  Min_circle mc(homes.begin(), homes.end(), true); // Randomize input.

  K::FT min_dist = numeric_limits<double>::max();

  for (auto it = mc.support_points_begin(); it < mc.support_points_end(); ++it) {
    vector<P> copy(homes);
    for (auto i = copy.begin(); i < copy.end(); ++i) {
      if (*i == *it) { copy.erase(i); }
    }

    Min_circle c(copy.begin(), copy.end(), true); // Randomize input.

    if (c.circle().squared_radius() < min_dist) {
      min_dist = c.circle().squared_radius();
    }
  }

  return ceil_to_double(sqrt(min_dist));
}

int main() {
  cout << setiosflags(ios::fixed) << setprecision(0);

  while (true) {
    int n;
    cin >> n;
    if (n == 0) { break; }

    vector<P> homes;
    for (int i = 0; i < n; ++i) {
      long x, y;
      cin >> x >> y;
      P h(x, y);
      homes.push_back(h);
    }

    cout << solve(homes) << endl;
  }
}
