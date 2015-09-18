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
  int ix = n * y + x;
  return get(ix);
}

void Matrix::set(int ix, int m) {
  elem[ix] = m;
}

void Matrix::set(int x, int y, int m) {
  int ix = n * y + x;
  set(ix, m);
}

int Matrix::size() {
  return n;
}

inline bool even(int n) {
  return !(n & 1); // The last bit of n must be 0.
}

int solve(Matrix& matrix) {
  int n = matrix.size();

  int sum[n+1][n+1];
  for (int ix = 0; ix <= n; ++ix) {
    sum[ix][0] = 0;
    sum[0][ix] = 0;
  }
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= n; ++j) {
      sum[i][j] = sum[i-1][j  ]
                + sum[i  ][j-1]
                - sum[i-1][j-1] // Counted doubly
                + matrix.get(i-1, j-1);
    }
  }

  int even_counter = 0;
  for (int i1 = 0; i1 <= n; ++i1) {
    for (int i2 = i1 + 1; i2 <= n; ++i2) {
      for (int j1 = 0; j1 <= n; ++j1) {
        for (int j2 = j1 + 1; j2 <= n; ++j2) {
          int s = 0;
          s += sum[i2][j2];
          s -= sum[i1][j2];
          s -= sum[i2][j1];
          s += sum[i1][j1]; // Counted doubly
          if (even(s)) { ++even_counter; }
        }
      }
    }
  }

  return even_counter;
}


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

