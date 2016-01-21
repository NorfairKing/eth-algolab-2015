#include <iostream>
#include <vector>

using namespace std;

struct request {
  int source;
  int target;
  int departure;
  int arrival;
  int profit;
};

int solve(int n, int s, vector<int>& cars, vector<request>& requests) {
  return 5;
}

int main() {
  int cases;
  cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int n, s;
    cin >> n >> s;

    vector<int> cs(s);
    for (int i = 0; i < s; ++i) {
      cin >> cs[i];
    }

    vector<request> rs(n);
    for (int i = 0; i < n; ++i) {
      cin >> rs[i].source;
      cin >> rs[i].target;
      cin >> rs[i].departure;
      cin >> rs[i].arrival;
      cin >> rs[i].profit;
    }

    cout << solve(n, s, cs, rs) << endl;
  }
}
