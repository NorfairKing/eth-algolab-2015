#include <iostream>
#include <queue>
#include <vector>


using namespace std;

// K is at most 16 so we will use an unsigned integer
class State {
  public:
    State(int n, int k);
    State(int n, int k, int pattern);
    State(const State& other);
    ~State();

    void set(int ix, bool val);
    void set(int part, int nr, bool val);
    void toggle(int ix);
    void toggle(int part, int nr);
    bool get(int ix);
    bool get(int part, int nr);

    uint getPart(int part);

    void switchFirstT(int t);
    int distance(State& other);
  private:
    int _k;
    int _n;
    int _p;
    vector<uint> state;

  friend ostream& operator<<(ostream& os, State& dt);
};

ostream& operator << (ostream& os, State& t) {
  for (int ix = t._n - 1; ix >= 0; --ix) {
    os << t.get(ix); // << " ";
  }
  os << endl;
  return os;
}

State::State(int n, int k) {
  _k = k;
  _n = n;
  _p = n / k;
  state = vector<uint>(_p);
  for (int p = 0; p < _p; ++p) {
    state[p] = 0;
  }
}

State::State(int n, int k, int pattern) : State(n, k) {
  for (int p = 0; p < n / k; ++p) {
    state[p] = pattern;
  }
}

State::State(const State& other) {
  this->_k = other._k;
  this->_n = other._n;
  this->_p = other._p;
  this->state = vector<uint>(other.state);
}

State::~State() {}

uint constexpr FULL_INT = -1; // Luckily they wrap around.

void State::set(int ix, bool val) {
  int part = ix / _k;
  int nr   = ix % _k;
  set(part, nr, val);
}

void State::set(int part, int nr, bool val) {
  // cout << "setting part: " << part << ", nr: " << nr << " to " << val << endl;
  // std::bitset<16> s(state[part]);
  // cout << s << endl;
  if (val) {
    // uint thing = (1 << nr);
    // std::bitset<16> t(thing);
    // std::cout << t << endl;
    // cout << "---------------- |" << endl;

    state[part] |= (1 << nr);
  } else {

    // uint thing = (FULL_INT ^ (1 << nr));
    // std::bitset<16> t(thing);
    // std::cout << t << endl;
    // cout << "---------------- &" << endl;

    state[part] &= (FULL_INT ^ (1 << nr));
  }
  // std::bitset<16> sp(state[part]);
  // cout << sp << endl;
}

void State::toggle(int ix) {
  int part = ix / _k;
  int nr   = ix % _k;
  return toggle(part, nr);
}

void State::toggle(int part, int nr) {
  state[part] ^= (1 << nr);
}

bool State::get(int ix) {
  int part = ix / _k;
  int nr   = ix % _k;
  return get(part, nr);
}

bool State::get(int part, int nr) {
  return state[part] & (1 << nr);
}

uint State::getPart(int part) {
  return state[part];
}

void State::switchFirstT(int t) {
  for (int p = _p - 1; p >= _p - t; --p) {
    state[p] ^= FULL_INT; // Flip them all.
  }
}

int State::distance(State& that) {
  int dist = 0;
  for (int p = 0; p < _p; ++p) {
    uint diff = this->getPart(p) ^ that.getPart(p);
    diff &= (1 << _k) - 1; // Only look at the last k bits.

    while(diff != 0){ // Brian Kernighan algorithm
      diff &= (diff - 1);
      dist++;
    }
  }
  return dist;
}

int solve(int n, int k, int x, State& start) {
  queue<pair<int, State>> q;
  // cout << start;
  State end(n, k, x);
  // cout << end;
  // int d = start.distance(end);
  // cout << d << endl;

  q.push(make_pair(0, start));

  while (!q.empty()) {
    pair<int, State> next = q.front();
    q.pop();
    int num  = next.first;
    State st = next.second;

    int dist = end.distance(st);
    // cout << num << " " << dist << " " << st;

    if (dist == 0) { return num; }

    for (int i = 0; i < n; ++i) {
      State other(st);
      other.toggle(i);
      // cout << "  " << other;
      pair<int,State> np = make_pair(num + 1, other);
      q.push(np);
    }

    for (int p = 1; p <= n / k; ++p) {
      State other(st);
      // cout << "  " << other;
      other.switchFirstT(p);
      pair<int,State> np = make_pair(num + 1, other);
      q.push(np);
    }

  }
  return -1;
}

int main() {
  int cases;
  cin >> cases;
  for (int c = 0; c < cases; ++c) {
    int n, k, x;
    cin >> n >> k >> x;

    State st(n, k);
    for (int i = 0; i < n; ++i) {
      int buf;
      cin >> buf;
      st.set(i, buf);
    }

    cout << solve(n, k, x, st) << endl;
  }
}
