#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

struct wizard {
  int boatlen;
  int ringpos;
};
static int freept;

bool operator < (const wizard& w1, const wizard& w2) {
  return std::max(freept + w1.boatlen, w1.ringpos)
       < std::max(freept + w2.boatlen, w2.ringpos);
}

int solve(std::vector<wizard>& ws) {
  int counter = 0;
  freept = INT_MIN;

  for (auto it = ws.begin(); it < ws.end(); ++it) {
    std::sort(it, ws.end());
    
    int leftend = std::max(freept, it->ringpos - it-> boatlen);
    int rightend = leftend + it->boatlen;
    if (rightend > it->ringpos + it->boatlen) { continue; }

    ++counter;
    freept = rightend;
  }
  return counter;
}

int main() {
  int cases;
  std::cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int wizards;
    std::cin >> wizards;
    std::vector<wizard> participants;
    for (int w = 0 ; w < wizards; ++w) {
      wizard wiz;
      std::cin >> wiz.boatlen;
      std::cin >> wiz.ringpos;
      participants.push_back(wiz);
    }
    std::cout << solve(participants) << std::endl;
  }
}
