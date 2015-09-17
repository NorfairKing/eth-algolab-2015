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

inline bool even(int n) {
  return (n & 1) ^ 1; // The last bit of n must be 0.
}

// The evenpairs problem
int onedevensums(std::vector<int> bits) {
  int even = 1;
  int odd = 0;
  int sum = 0;
  int n = bits.size();
  for (int i = 0; i < n; ++i) {
    sum = (sum + bits[i]) % 2;
    if (sum == 0) even++;
    else odd++;
  }
  return even * (even-1)/2 + odd * (odd-1)/2;
}

// 
int solve(Matrix& matrix) {
  int res = 0;
  int n = matrix.size();
  for (int i = 0; i < n; ++i) {
    std::vector<int> oneDim(n);
    for (int j = i; j < n; j++) {
      for (int k = 0; k < n; k++) {
        oneDim[k] = (oneDim[k] + matrix.get(j, k)) % 2;
      }
      res += onedevensums(oneDim);
    }
  }
  return res;
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

