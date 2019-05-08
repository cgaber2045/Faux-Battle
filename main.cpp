#include "battle.h"
#include <chrono>

int main() {
  auto start = chrono::steady_clock::now();
  vector<Unit> myUnits = readStats("readStatsInputSample.txt");
  vector<Unit> theirUnits = myUnits;

  unordered_map<string, Order> order = fight(myUnits, theirUnits);

  auto finish = chrono::steady_clock::now();

  auto diff = finish - start;

  cout << "My Units: " << endl;
  for (Unit x: myUnits) cout << x.ID << ", ";
  cout << endl << endl;

  cout << "Their Units: " << endl;
  for (Unit x: theirUnits) cout << x.ID << ", ";
  cout << endl << endl;

  cout << "STRAT: " << endl;
  for (auto unit : order) {
    cout << unit.first << ": {";
    for (string x: unit.second.targetIDs) {
      cout << x << ", ";
    }
    cout << "}" << endl;
    cout << unit.first << ": {";
    for (string x: unit.second.actions) {
      cout << x << ", ";
    }
    cout << "}" << endl << endl;
  }

  cout << "Runtime: " << chrono::duration <double, milli> (diff).count() << " ms" << endl;
}
