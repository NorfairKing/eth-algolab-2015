#include <iostream>
#include <vector>
#include <climits>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

using namespace std;
using namespace CGAL;

// choose exact integral type
#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>
typedef Gmpz ET;
#else
#include <CGAL/MP_Float.h>
typedef MP_Float ET;
#endif

// program and solution types
typedef Quadratic_program<int> Program;
typedef Quadratic_program_solution<ET> Solution;


pair<int, int> solve(int n, int m, int s,
                     vector<int>& limits,
                     vector<int>& states,
                     vector<vector<int>>& bids) {

  Program p(SMALLER
      , true, 0 // Boolean variables
      , true, 1);

  // i is buyer
  // j is house

  // Default is to minimise
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      p.set_c(i * m + j, - bids[i][j]);
    }
  }

  int eq = 0;
  // Every house can only be bought once.
  for (int j = 0; j < m; ++j) {
    for (int i = 0; i < n; ++i) {
      p.set_a(i * m + j, eq, 1);
    }
    p.set_r(eq, SMALLER);
    p.set_b(eq, 1);
    ++eq;
  }

  // Every buyer can only buy one house.
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      p.set_a(i * m + j, eq, 1);
    }
    p.set_r(eq, SMALLER);
    p.set_b(eq, 1);
    ++eq;
  }

  for (int k = 0; k < s; ++k) { // state
    for (int j = 0; j < m; ++j) { // land
      if (states[j] != k) { continue; } // land is not in the state
      for (int i = 0; i < n; ++i) {
        p.set_a(i * m + j, eq, 1);
      }
    }
    p.set_r(eq, SMALLER);
    p.set_b(eq, limits[k]);
    ++eq;
  }

  Solution sol = solve_quadratic_program(p, ET());
  // assert(sol.solves_quadratic_program(p));
  
  int profit = - to_double(sol.objective_value());

  int sold = 0;
  for (auto it = sol.variable_values_begin(); it != sol.variable_values_end(); ++it) {
    sold += to_double(*it);
  }

  return make_pair(sold, profit);
}

int main() {
  int cases;
  cin >> cases;

  for (int c = 0; c < cases; ++c) {
    int n, m, s;
    cin >> n >> m >> s;

    vector<int> ls(s);
    for (int i = 0; i < s; ++i) {
      cin >> ls[i];
    }

    vector<int> ss(m);
    for (int i = 0; i < m; ++i) {
      cin >> ss[i];
      ss[i]--; //zero indexed
    }

    vector<vector<int>> bs(n, vector<int>(m));
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        cin >> bs[i][j];
      }
    }

    pair<int, int> sol = solve(n, m, s, ls, ss, bs);
    cout << sol.first << " " << sol.second << endl;
  }
}
