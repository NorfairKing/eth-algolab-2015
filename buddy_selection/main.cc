#include <iostream>
#include <vector>
#include <map>

using namespace std;

void print_student(vector<int>& student) {
  int n = student.size();
  for (int i = 0; i < n; ++i) {
    cout << student[i] << " ";
  }
  cout << endl;
}

bool solve(vector<vector<int>> students, int total_chars, int min_chars) {
  return true;
}

int main() {
  int cases;
  cin >> cases;
  for (int tcase = 0; tcase < cases; ++tcase) {
    int n, c, f;
    cin >> n >> c >> f;

    vector<vector<int>> students(n);
    int wordcount = 0; // Zero indexed words
    map<string, int> strmap;
    for (int i = 0; i < n; ++i) {
      vector<int> student(c);
      for (int j = 0; j < c; ++j) {
        string s;
        cin >> s;
        //cout << s << endl;

        if (strmap.find(s) == strmap.end()) { // Doesn't contain s yet
          strmap.insert(make_pair(s, wordcount));
          ++wordcount;
          //cout << s << " didn't exist yet." << " adding as " << wordcount << endl;
        }
        student[j] = strmap[s];
      }
      // print_student(student);
      students[i] = student;
    }
    // cout << wordcount << endl;
    bool result = solve(students, wordcount, f);

    if (result) {
      cout << "optimal" << endl;
    } else {
      cout << "not optimal" << endl;
    }
  }
}
