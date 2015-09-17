#include <iostream>
#include <vector>

int gcd(int a, int b) {
  if (b == 0) { return a; }
  return gcd(b, a % b);
}

int lcm(int a, int b) {
  return a * b / gcd(a, b);
}

class Solution {
  private:
    int l;
    int m;
    int n;
    int a;
    int b;
  public:
    Solution(int l, int m, int n);
    Solution(const Solution& that );
    ~Solution();
    int getA();
    int getB();
    int getError();
    void addA();
    void solve();
    friend std::ostream& operator << (std::ostream& out, Solution& board);
};

Solution::Solution(int l, int m, int n) {
  this->l = l;
  this->m = m;
  this->n = n;
  a = 0;
  b = 0;
}

Solution::Solution(const Solution& that) {
  this->l = that.l;
  this->m = that.m;
  this->n = that.n;
  this->a = that.a;
  this->b = that.b;
}

Solution::~Solution() {}

int Solution::getA() {
  return this->a;
}

int Solution::getB() {
  return this->b;
}

int Solution::getError() {
  return l - a*m - b*n;
}

void Solution::addA() {
  a++;
  while (getError() < 0) { b--; }
}

void Solution::solve() {
  b = l / n; // As many n's as possible
  a = getError() / m; // Fill rest with m's

  if (getError() == 0) { return; } 

  int lm = lcm(m,n);
  if (n % m == 0) { return; }

  Solution bestSolution = *this;
  Solution other(*this);
  for (int i = 0; i <= lm / m; ++i) { // There's no point in going past lm.
    other.addA();
    if (other.getError() < this->getError()) { // < not <= because we want as many n's as possible.
      //std::cout << "better" << std::endl;
      bestSolution = other;
    } else {
      //std::cout << "not better" << std::endl;
    }
    if (bestSolution.getError() == 0) { break; }
  }

  this->a = bestSolution.getA();
  this->b = bestSolution.getB();
}

std::ostream& operator << (std::ostream &out, Solution &sol) {
  out << sol.a << " " << sol.b << " " << sol.getError() << std::endl;
  return out;
}


int main() {
  std::ios_base::sync_with_stdio(false);

  int cases;
  std::cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int l, m, n;
    std::cin >> l;
    std::cin >> m;
    std::cin >> n;

    Solution s(l,m,n);
    s.solve();
    std::cout << s;
  }
  return 0;
}

