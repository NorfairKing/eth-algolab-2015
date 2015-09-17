#include <iostream>
#include <vector>

class Matrix {
  private:
    int n;
    int * elem;
  public:
    Matrix(int size);
    ~Matrix();
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

int solve(const Matrix& bits);

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
    }
    std::cout << solve(matrix) << std::endl;
  }
}

inline bool even(int n) {
  return (n & 1) ^ 1; // The last bit of n must be 0.
}

// Define s[i] as \sum_{i=0}^n bits[i]
// For each couple i,j the sum of elements between i and j can be found by:
// \sum_{n=i}^j bits[i] = s[j]-s[i]
//
// For a given j:
// - if s[j] is even, s[j]-s[i] is only going to be even is s[i] is also even
// - if s[j] is odd, s[j]-s[i] is only going to be even if s[i] is also odd
//
// Define e[j] as the number of even sums below j
//
// For a given j:
// - if s[j] is even, count e[j] even sums.
// - if s[j] is odd, count j-e[j] even sums.
int solve(const Matrix& matrix) {
  return 1;
}
