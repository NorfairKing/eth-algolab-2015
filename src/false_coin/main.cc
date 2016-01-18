#include <iostream>
#include <vector>
#include <algorithm>

enum outcome { LT, EQ, GT };

struct weighing {
  std::vector<int> left;
  std::vector<int> right;
  outcome out;
};

void normalise(std::vector<weighing>& ws) {
  int k = ws.size();
  for (int i = 0; i < k; ++i) {
    weighing w = ws[i];
    if (w.out == GT) {
      std::vector<int> tmp = w.left;
      w.left = w.right;
      w.right = tmp;
      w.out = LT;
      ws[i] = w;
    }
  }
}

std::vector<bool> get_possibles(std::vector<weighing>& ws, int n) {
  std::vector<bool> possibles(n + 1, true);
  int k = ws.size();
  for (int i = 0; i < k; ++i) {
    if (ws[i].out == EQ) {
      int pan_size = ws[i].left.size();
      for (int coin = 0; coin < pan_size; ++coin) {
        possibles[ws[i].left[coin]]  = false;
        possibles[ws[i].right[coin]] = false;
      }
    }
  }
  return possibles;
}

std::vector<int> intersect(std::vector<int>& suspects, std::vector<int>& coins) {
  std::sort(coins.begin(), coins.end());
  std::vector<int> result(suspects.size());
  std::vector<int>::iterator it;
  it = std::set_intersection(suspects.begin(), suspects.end(),
                             coins.begin(), coins.end(),
                             result.begin());
  result.resize(it - result.begin());
  return result;
}

int solve(std::vector<weighing> ws, int n) {
  normalise(ws);
  std::vector<bool> possibles = get_possibles(ws, n);

  std::vector<int> left_suspects;
  std::vector<int> right_suspects;

  for (int i = 1; i <= n; ++i) {
    if (possibles[i]) { left_suspects.push_back(i); }
    if (possibles[i]) { right_suspects.push_back(i); }
  }

  std::sort(left_suspects.begin(), left_suspects.end());
  std::sort(right_suspects.begin(), right_suspects.end());

  int k = ws.size();
  for (int i = 0; i < k; ++i) {
    weighing w = ws[i];
    if (w.out != LT) { continue; }

    left_suspects = intersect(left_suspects, w.left);
    right_suspects = intersect(right_suspects, w.right);
  }

  int total_suspects = left_suspects.size() + right_suspects.size();
  if (total_suspects != 1){
    return 0;
  }
  if (left_suspects.size() == 1) {
    return left_suspects[0];
  }
  if (right_suspects.size() == 1) {
    return right_suspects[0];
  }
  return 0;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int cases;
  std::cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int n, k;
    std::cin >> n;
    std::cin >> k;
    std::cin.get(); // get the newline

    std::vector<weighing> ws;

    for (int i = 0; i < k; ++i) {
      int p;
      std::cin >> p;

      int buf;
      std::vector<int> left;
      std::vector<int> right;
      for (int j = 0; j < p; ++j) {
        std::cin >> buf;
        left.push_back(buf);
      }
      for (int j = 0; j < p; ++j) {
        std::cin >> buf;
        right.push_back(buf);
      }
      std::cin.get(); // get the space
      char out = std::cin.get();

      weighing w;
      w.left = left;
      w.right = right;
      switch (out) {
        case '<': w.out = LT; break;
        case '>': w.out = GT; break;
        case '=': w.out = EQ; break;
      }

      ws.push_back(w);
    }
    std::cout << solve(ws, n) << std::endl;
  }
}
