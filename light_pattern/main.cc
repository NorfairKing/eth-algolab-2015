#include <iostream>
#include <bitset>
#include <vector>


using namespace std;

// K is at most 16 so we will use an unsigned integer
class State {
  public:
    State(int n, int k);
    State( State& other );
    ~State();

    void set(int ix, bool val);
    void set(int part, int nr, bool val);
    bool get(int ix);
    bool get(int part, int nr);

    void switchFirstT(int t);
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
}

State::State(State& other) {
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

bool State::get(int ix) {
  int part = ix / _k;
  int nr   = ix % _k;
  return get(part, nr);
}

bool State::get(int part, int nr) {
  return state[part] & (1 << nr);
}

void State::switchFirstT(int t) {
  for (int i = 0; i < t; ++i) {
    state[i] ^= FULL_INT; // Flip them all.
  }
}

int solve(int n, int k, int x, State& start) {
  cout << start;
  start.switchFirstT(2);
  cout << start;


  return 5;
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
