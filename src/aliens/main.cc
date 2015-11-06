#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

struct interval {
  int first;
  int last;
};

bool operator < (const interval& i1, const interval& i2) {
  if (i1.first == i2.first) {
    return i1.last > i2.last;
  } else {
    return i1.first < i2.first;
  }
}

bool operator == (const interval& i1, const interval& i2) {
  return i1.first == i2.first && i1.last == i2.last;
}

int solve (int humans, int aliens, std::vector<interval>& ais) {
  std::sort(ais.begin(), ais.end());

  int left = humans;
  int right = -1;
  for (int a = 0; a < aliens; ++a) {
    interval& i = ais[a];
    if (i.first > right + 1) { return 0; }
    right = std::max(i.last, right);
    left = std::min(i.first, left);
  }
  if (right < humans - 1) { return 0; }
  if (left > 0) { return 0; }

  int counter = 0;
  int rightmost = -1;
  for (int a = 0; a < aliens; ++a) {
    interval& i = ais[a];
    if (i.last <= rightmost) { continue; }
    rightmost = std::max(rightmost, i.last);
    if (a < aliens - 1 && i == ais[a + 1]) { continue; }
    counter++;
  }

  return counter;
}

int main() {
  int cases;
  std::cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int n, m;
    std::cin >> n; // aliens
    std::cin >> m; // humans

    std::vector<interval> intervals;
    for (int i = 0; i < n; ++i) {
      interval in;
      std::cin >> in.first;
      std::cin >> in.last;
      --in.first; // zero indexed 
      --in.last; // zero indexed
      intervals.push_back(in);
    }
    std::cout << solve(m, n, intervals) << std::endl;
  }
}
