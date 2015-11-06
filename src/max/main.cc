#include <iostream>
#include <cassert>
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

void solve(int p, int a, int b) {
  std::cout <<  std::setprecision(15); // To truly output doubles as ints
  const int X = 0;
  const int Y = 1;
  const int Z = 2;
  if (p == 1) {
    Program qp (SMALLER
        , true , 0 // Lower bound 0
        , false, 0 // No upper bounds
        );

    Program lp(SMALLER, true, 0, false, 0);
    lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);
    lp.set_a(X, 1,  4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b);
    lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);

    // Double D
    lp.set_c(Y, -b);
    lp.set_d(X, X, 2 * a);
    Solution s = solve_quadratic_program(lp, ET());

    if (s.is_optimal()) {
      cout << floor((-1) * to_double(s.objective_value())) << endl;
    }  else if (s.is_unbounded()) {
      cout << "unbounded" << endl;
    } else {
      cout << "no" << endl;;
    }
  }
  if (p == 2) {
    Program qp(LARGER  // Mapping (x, y, z) -> (x, y, z^2)
        , false, 0     // No lower bound
        , true , 0);   // Upper bound 0
    qp.set_a(X, 0,  1); qp.set_a(Y, 0, 1);                    qp.set_b(0, -4);
    qp.set_a(X, 1,  4); qp.set_a(Y, 1, 2); qp.set_a(Z, 1, 1); qp.set_b(1, -a*b);
    qp.set_a(X, 2, -1); qp.set_a(Y, 2, 1);                    qp.set_b(2, -1);

    qp.set_l(Z, true, 0);   // Lower bound for z: 0
    qp.set_u(Z, false, 0);  // No upper bound for z

    // Double D
    qp.set_d(X, X, 2*a);
    qp.set_c(Y, b);
    qp.set_d(Z, Z, 2);

    Solution s = solve_quadratic_program(qp, ET());

    if (s.is_optimal()) {
      if (ceil(to_double(s.objective_value())) == -0) {
        cout << 0 << endl;
      } else {
        cout << ceil(to_double(s.objective_value())) << endl;
      }
    } else if (s.is_unbounded()) {
      cout << "unbounded" << endl;
    } else {
      cout << "no" << endl;
    }
  }
}

int main() {
  while (true) {
    int p, a, b;
    cin >> p;
    if (p == 0) { break; }
    cin >> a;
    cin >> b;

    solve(p, a, b);
  }
}
