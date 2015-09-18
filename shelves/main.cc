#include <iostream>
#include <vector>
#include <cmath>

void solve(long l, long m, long n);
void better(long l, long m, long n);
void clever(long l, long m, long n);

// Try the O(l/n) algorithm if n is larger than sqrt(l), otherwise try the O(n) algorithm.
// O(sqrt(l))
int main() {
  std::ios_base::sync_with_stdio(false);

  int cases;
  std::cin >> cases;
  for (int c = 0; c < cases; ++c) {
    long l, m, n;
    std::cin >> l;
    std::cin >> m;
    std::cin >> n;

    solve(l, m, n);
  }
}

void better(long l, long m, long n) {
  long e, a, b;
  e = l;
  a = 0;
  b = 0;
  // try every b and keep the best.
  for(int i = 0; i * n <= l; ++i) {
    // Place i shelves of length n.
    long left = l - i * n;
    long leftover = left % m;
    long j = left / m;
    if (leftover <= e) {
      e = leftover;
      a = j;
      b = i;
    }
  }
  std::cout << a << " " << b << " " << e << std::endl;
}

void solve(long l, long m, long n){
  if (n * n > l) {
    better(l, m, n);
  } else {
    clever(l, m, n);
  }
}

void clever(long l, long m, long n) {
  long min[n];
  for (int i = 0; i < n; ++i) { min[i] = -1; } // Dummy value to signify unvisited

  long at = 0;
  min[0] = 0;
  while (true) {
    long next = (at + m) % n;
    if(min[next] != -1) { break; }
    min[next] = min[at] + 1;
    at = next;
  }

  long a, b, e;
  for (e = 0; e < n; ++e) {
    int used = l - e;
    a = min[used % n];
    if (a == -1 || a * m > used) { continue; } // impossible or impossible within l
    b = (used - a * m) / n;
    break;
  }
  std::cout << a << " " << b << " " << e << std::endl;
}
