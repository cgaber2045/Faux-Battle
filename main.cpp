#include "logistics.h"
#include <chrono>

int main() {
  vector<pair<string,double>> start = {pair<string,double>("a", 1000), pair<string,double>("b", 10)};
  vector<pair<string,double>> end = {pair<string,double>("h", 10), pair<string,double>("z", 100)};

  unordered_map<int,double> list = organizeLogistics(start, end);
  for (auto x: list) {
    cout << x.first << ": " << x.second << endl;
  }

  return 0;
}
