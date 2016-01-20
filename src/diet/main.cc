#include <iostream>
#include <vector>
#include <boost/optional.hpp>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

using namespace std;
using namespace CGAL;
using namespace boost;

#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>
typedef Gmpz ET;
#else
#include <CGAL/MP_Float.h>
typedef MP_Float ET;
#endif

typedef Quadratic_program<int> Program;
typedef Quadratic_program_solution<ET> Solution;

struct nut {
  int mn;
  int mx;
};

struct food {
  int price;
  vector<int> nuts;
};


// This is an LP problem!
// ======================
//
// The objective is to minimise
//
// sum_i o_i foods[i].price
//
//
// Under these constraints
//
// for all nutritions i
//   sum_j o_j foods[i].nuts[j] >= nuts[i].mn
//   sum_j o_j foods[i].nuts[j] >= nuts[i].mx
//
// n nutritients
// m foods
optional<double> solve(int n, int m, vector<nut>& nuts, vector<food>& foods) {
  Program p(SMALLER
      , true , 0 // Lower bound 0
      , false, 0 // No upper bounds
      );
  
  // Default is to minimise the objective function.
  for (int j = 0; j < m; ++j) {
    p.set_c(j, foods[j].price);
  }

  for (int i = 0; i < n; ++i) {
    p.set_r(    i, SMALLER);
    p.set_b(    i, nuts[i].mx);

    p.set_r(n + i, LARGER);
    p.set_b(n + i, nuts[i].mn);

    for (int j = 0; j < m; ++j) {
      p.set_a(j,     i, foods[j].nuts[i]);
      p.set_a(j, n + i, foods[j].nuts[i]);
    }
  }


  Solution s = CGAL::solve_quadratic_program(p, ET());
  assert (s.solves_quadratic_program(p));
  if (!s.is_optimal() || s.is_unbounded()) {
    return none;
  } else {
    return floor(to_double(s.objective_value()));
  }
}

int main () {
  cout << setprecision(15);
  while (true) {
    int n, m;
    cin >> n >> m;
    if (n == 0 && m == 0) { break; }

    vector<nut> ns(n);
    for (int i = 0; i < n; ++i) {
      cin >> ns[i].mn;
      cin >> ns[i].mx;
    }

    vector<food> fs(m);
    for (int i = 0; i < m; ++i) {
      cin >> fs[i].price;

      for (int j = 0; j < n; ++j) {
        int cij;
        cin >> cij;
        fs[i].nuts.push_back(cij);
      }
    }
    
    optional<double> result = solve(n, m, ns, fs);
    if (result) {
      cout << result.get();
    } else {
      cout << "No such diet.";
    }
    cout << endl;
  }
}
