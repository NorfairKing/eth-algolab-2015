#include <iostream>
#include <vector>

class Matrix {
  private:
    int n;
    int * elem;
  public:
    Matrix(int size);
    ~Matrix();
    int size();
    int get(int ix);
    int get(int x, int y);
    void set(int ix, int m);
    void set(int x, int y, int m);
};

Matrix::Matrix(int size) {
  n = size;
  elem = new int[n * n];
  for (int x = 0; x < n; ++x) {
    for (int y = 0; y < n; ++y) {
      set(x,y,0);
    }
  }
}

Matrix::~Matrix() {
  delete [] elem;
}

int Matrix::get(int ix) {
  return elem[ix];
}
int Matrix::get(int x, int y) {
  int ix = n * x + y;
  return get(ix);
}

void Matrix::set(int ix, int m) {
  elem[ix] = m;
}

void Matrix::set(int x, int y, int m) {
  int ix = n * x + y;
  set(ix, m);
}

int Matrix::size() {
  return n;
}

int solve(Matrix& bits);

int main() {
  int cases;
  std::cin >> cases;
  for (int c; c < cases; ++c) {
    int n;
    std::cin >> n;

    Matrix matrix(n);
    int buf;
    for (int x = 0; x < n; ++x) {
      for (int y = 0; y < n; ++y) {
        std::cin >> buf;
        matrix.set(x, y, buf);
      }
      std::cin.get(); // get the newline
    }
    std::cout << solve(matrix) << std::endl;
  }
}

inline bool even(int n) {
  return (n & 1) ^ 1; // The last bit of n must be 0.
}

// Better solution: precompute sums
// make an matrix sum where sum[i][j] contains the sum of all elements in the corner.
// Define e[i][j] as the number of even sums below [i][j].
//    
int solve(Matrix& matrix) {
  int n = matrix.size();
  int even_counter = 0;

  int sum[n+1][n+1];
  for (int ix = 0; ix <= n; ++ix) {
    sum[ix][0] = 0;
    sum[0][ix] = 0;
  }
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      sum[i+1][j+1] = sum[i  ][j+1]
                    + sum[i+1][j  ]
                    - sum[i  ][j  ] // Counted doubly
                    + matrix.get(i, j);
    }
  }

  int evens[n+1][n+1];
  int odds[n+1][n+1];
  for (int ix = 0; ix <= n; ++ix) {
    evens[ix][0] = 0;
    evens[0][ix] = 0;
    odds[ix][0] = 0;
    odds[0][ix] = 0;
  }
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      evens[i+1][j+1] = evens[i  ][j+1]
                      + evens[i+1][j  ]
                      - evens[i  ][j  ] // Counted doubly
                      + even(sum[i+1][j+1]);
      odds[i+1][j+1]  = odds[i  ][j+1]
                      + odds[i+1][j  ]
                      - odds[i  ][j  ] // Counted doubly
                      + !even(sum[i+1][j+1]);
    }
  }

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cout << matrix.get(i, j) << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  for (int i = 0; i <= n; ++i) {
    for (int j = 0; j <= n; ++j) {
      std::cout << sum[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  for (int i = 0; i <= n; ++i) {
    for (int j = 0; j <= n; ++j) {
      std::cout << evens[i][j] << " ";
    }
    std::cout << std::endl;
  }
  for (int i = 0; i <= n; ++i) {
    for (int j = 0; j <= n; ++j) {
      std::cout << odds[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;



  // In what conditions will s be even?
  //
  // s = s[i2][j2] - s[i1][j2] - s[i2][j1] + s[i1][j1]
  //
  // s[i2][j2] even, s[i1][j2] even:
  //  s will be even iff both s[i2][j1] and s[i1][j1] are even.
  //
  // s[i2][j2] even, s[i1][j2] odd:
  //  s will be even if one of s[i2][j1] and s[i1][j1] are even but not the other.
  //
  // s[i2][j2] odd, s[i1][j2] even:
  //  s will be even if one of s[i2][j1] and s[i1][j1] are even but not the other.
  //
  // s[i2][j2] odd, s[i1][j2] odd:
  //  s will be even if both s[i2][j1] and s[i1][j1] are odd.
  //
  // define e[i][j] as the number of even sums below [i][j].
  //
  // - if s[i2][j2] is even, count
  for (int i2 = 0; i2 <= n; ++i2) {
    for (int j2 = 0; j2 <= n; ++j2) {
      for (int i1 = 0; i1 < i2; ++i1) {

        //int l = i1*j2;
        //int r = i2*j2;

        if (even(sum[i2][j2])) {
          if (even(sum[i1][j2])) {
            even_counter += evens[i1][j2];
            even_counter += evens[i2][j2];
            even_counter -= odds[i1][j2];
            even_counter -= odds[i2][j2];
          } else {
            even_counter += (evens[i1][j2] + odds[i2][j2]);
            even_counter += (odds[i1][j2] + evens[i2][j2]);
          }
        } else {
          if (even(sum[i1][j2])) {
            even_counter += (evens[i1][j2] + odds[i2][j2]);
            even_counter += (odds[i1][j2] + evens[i2][j2]);
          } else {
            even_counter += odds[i1][j2] + odds[i2][j2];
          }
        }
      }
    }
  }

  return even_counter;
}
