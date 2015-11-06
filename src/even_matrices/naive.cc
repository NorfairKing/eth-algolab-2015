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
  int even_counter = 0;
  int n = matrix.size();

  for (int i1 = 0; i1 < n; ++i1) {
    for (int j1 = 0; j1 < n; ++j1) {
      for (int i2 = i1; i2 < n; ++i2) {
        for (int j2 = j1; j2 < n; ++j2) {
          int sum = 0;
          for (int ki = i1; ki <= i2; ++ki) {
            for (int kj = j1; kj <= j2; ++kj) {
              sum ^= matrix.get(ki, kj);
            }
          }
          if (even(sum)) { ++even_counter; }
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

