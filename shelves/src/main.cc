#include <iostream>
#include <vector>
#include <cmath>

void brute_force(long l, long m, long n);
void smart(long l, long m, long n);

int main() {
  std::ios_base::sync_with_stdio(false);

  int cases;
  std::cin >> cases;
  for (int c = 0; c < cases; ++c) {
    long l, m, n;
    std::cin >> l;
    std::cin >> m;
    std::cin >> n;

    if (n * n > l) {
      brute_force(l, m, n);
    } else {
      smart(l, m, n);
    }
  }
}

void brute_force(long l, long m, long n) {
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
    if (leftover < e || (leftover == e && i > b)) {
      e = leftover;
      a = j;
      b = i;
    }
  }
  std::cout << a << " " << b << " " << e << std::endl;
}

void smart(long l, long m, long n) {
  long e, a, b;
  e = l;
  a = 0;
  b = 0;
  // min[i] holds the lowest number of m's that add up to a sum that is equivalent to i, mod n
  long min[n];
  for (int i = 0; i < n; ++i) { min[i] = -1; }
  min[0] = 0;
  int at = 0;
  while(true) {
    int next = (int) ((at + m) % n);
    if(min[next] != -1) { break; } // We have cycled around to a value we already found, so no point in adding m anymore.
    min[next] = min[at] + 1;
    at = next;
  }

  for (int left = 0; left < n; ++left) {
    // left is how many spaces we fail to fill
    int totalUsed = (int)(l - left);
    // Take a bunch of m's to get a value equivalent to totalUsed mod n -> we want to take as few m's as possible.
    long countM = min[totalUsed % n];
    if (countM == -1 || countM * m > totalUsed) { continue; } // Such a value either cannot be reached or is not reached until after totalUsed.
    long countN = (totalUsed - countM * m) / n;
    e = left;
    a = countM;
    b = countN;
    break; // All other possible solutions will have more leftover than this, so just break now.
  }
  std::cout << a << " " << b << " " << e << std::endl;
}
