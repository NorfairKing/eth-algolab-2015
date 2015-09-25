#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

struct wizard {
  int boatlen;
  int ringpos;
};

bool operator < (const wizard& w1, const wizard& w2) {
  return w1.ringpos < w2.ringpos;
}
std::ostream& operator << (std::ostream &out, wizard& w) {
  out << "Boat length: " << w.boatlen << "  Ring pos: " << w.ringpos << std::endl;
  return out;
}

int solve(std::vector<wizard>& ws) {
  int counter = 0;
  int right = INT_MIN;
  int second_right = INT_MIN;

  std::sort(ws.begin(), ws.end());
  for (auto it = ws.begin(); it < ws.end(); ++it) {
    if (it->ringpos >= right) {
      // We can just place the boat
      second_right = right;
      right = std::max(it->ringpos, right + it->boatlen);
      ++counter;
    } else {
      int first_free = std::max(it->ringpos, second_right + it->boatlen);
      if (first_free <= right) {
        // Replace the last boat with this one.
        right = first_free;
      } else {
        // Skip this boat
      }
    }
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
