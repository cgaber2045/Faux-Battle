/*
* Name: Chris Gaber
* Date Submitted: 4/24/19
* Assignment Name: Faux Battle
*/

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

struct Unit {
	string ID;
	string unitClass;
	double currentHP;
	int speed;
	double power;
	tuple<int,int,int> currentLocation;
	int range;

	Unit(string id, string cl, double chp, int spd, double pwr, tuple<int,int,int> curloc, int rng) :
	ID(id), unitClass(cl), currentHP(chp), speed(spd), power(pwr), currentLocation(curloc), range(rng) {}
};

struct Order {
	vector<string> actions; // contains consecutive moves up to the value of speed
	vector<char> moveSequence; // if moving, contains 'L','R','U','D' to give in what direction to move
	vector<string> targetIDs; // a vector giving the ID values of each unit to be targeted (in order).
	// to target the same unit until destruction, then move on, put the first unit's targetID X times,
	// where X is your speed; then put the next unit's targetID.
	string targetClass;  // if no targetIDs (or if all targetIDs destroyed), unit will target whatever
	// unit class is specified here until they are all destroyed.
};

unordered_map<string, string> bestAgainst;
vector<Unit> Units;
bool lowHealth(Unit l, Unit r);
vector<Unit> readStats(string);
unordered_map<string, Order> fight(vector <Unit>, vector<Unit>);

// Compares two units overall power. Finds the highest priority units.
bool killFactorCalc(Unit x, Unit y) {
  return (x.power*.5 + x.speed*2)/x.currentHP > (y.power*.5 + y.speed*2)/y.currentHP;
}

unordered_map<string, Order> fight(vector<Unit> yourUnits, vector<Unit> theirUnits) {
  unordered_map<string, Order> strategy;

  // Sort their troops so highest power are at start.
  sort(theirUnits.begin(), theirUnits.end(), killFactorCalc);

	// for each of our units
  for (Unit friendly: yourUnits) {
    Order newOrder;

		// attack the enemy with the highest kill score
    for (Unit enemy: theirUnits) {
			// if our units class is the best against this enemy, move it to front of attack
      if (bestAgainst.at(friendly.unitClass) == enemy.unitClass) {
        newOrder.targetClass = enemy.unitClass;
        for (int i = friendly.speed; i > 0; i--)
        newOrder.targetIDs.insert(newOrder.targetIDs.begin(), enemy.ID);
        continue;
      }

			// attack until dead
      for (int i = friendly.speed; i > 0; i--) newOrder.targetIDs.push_back(enemy.ID);
    }

		// no move implementation, so add fight mechanic
    while(newOrder.actions.size() != friendly.speed) newOrder.actions.push_back("fight");

    strategy.insert(pair<string, Order>(friendly.ID, newOrder));
  }

  return strategy;
}

vector<Unit> readStats(string file) {
  ifstream inFile(file);

  string ID;
  string unitClass;
  double currentHP;
  int speed;
  double power;
  int range;
  string best_against;
  tuple<int,int,int> loc;

  while (inFile >> ID >> unitClass >> currentHP >> speed >> power >> range >> best_against) {
    Unit newUnit(ID, unitClass, currentHP, speed, power, loc, range);
    bestAgainst.insert(pair<string,string>(unitClass, best_against));
    Units.push_back(newUnit);
  }

  return Units;
}
