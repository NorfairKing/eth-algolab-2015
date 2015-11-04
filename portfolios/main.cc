#include <iostream>
#include <vector>
#include <boost/optional.hpp>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

using namespace std;
using namespace CGAL;

#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>
typedef Gmpz ET;
#else
#include <CGAL/MP_Float.h>
typedef MP_Float ET;
#endif

typedef Quadratic_program<int> Program;
typedef Quadratic_program_solution<ET> Solution;

struct investor {
  int maxcost;
  int minret;
  int maxvar;
};

struct asset {
  int cost;
  int ret;
};

// This is a quadratic programming problem!
//
// Let o_i be the fraction of shared of asset i to buy.
//
// For every invesor, we minimize the total variance
// sum_i sum_j o_i * o_j * var[i][j]
//
// With respect to these constraints:
// sum_i o_i * assets[i].cost <= investors[i].maxcost
// sum_i o_i * assets[i].ret >= investors[i].minret
//
// If the objective value is smaller than maxvar_i, then add a true to the vector of outputs.
vector<bool> solve(int n, int m
    , vector<asset>& assets
    , vector<vector<int>>& var
    , vector<investor>& investors) {

  vector<bool> result(m, false);

  Program p(SMALLER
      , true , 0 // shares are fractional but nonnegative
      , false, 0
      );

  // Specify 2*D
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j <= i; ++j) { // enforce j <= i
      p.set_d(i, j, 2 * var[i][j]);
    }
  }

  for (int i = 0; i < n; ++i) {
    p.set_a(i, 0, assets[i].cost);
    p.set_a(i, 1, assets[i].ret);
  }
  p.set_r(0, SMALLER);
  p.set_r(1, LARGER);

  for (int i = 0; i < m; ++i) {
    p.set_b(0, investors[i].maxcost);
    p.set_b(1, investors[i].minret);

    Solution s = solve_quadratic_program(p, ET());
    // This is a costly action, don't do it when submitting!
    // assert(s.solves_quadratic_program(p));

    if (s.is_optimal() && s.objective_value() <= investors[i].maxvar) {
      result[i] = true;
    }
  }

  return result;
}

int main() {

  while (true) {
    int n, m;
    cin >> n >> m;
    if (n == 0 && m == 0) { break; }
    
    vector<asset> as(n);
    for (int i = 0; i < n; ++i) {
      cin >> as[i].cost;
      cin >> as[i].ret;
    }

    vector<vector<int>> vars(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        cin >> vars[i][j];
      }
    }

    vector<investor> is(m);
    for (int i = 0; i < m; ++i) {
      cin >> is[i].maxcost;
      cin >> is[i].minret;
      cin >> is[i].maxvar;
    }

    vector<bool> solution = solve(n, m, as, vars, is);
    for (int i = 0; i < m; ++i) {
      if (solution[i]) {
        cout << "Yes." << endl;
      } else {
        cout << "No." << endl;
      }
    }
  }
}
