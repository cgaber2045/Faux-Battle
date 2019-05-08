/*
 * Name: Chris Gaber
 * Date Submitted: 4/24/19
 * Assignment Name: Logistics
 */

#pragma once

#include <set>
#include <queue>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>
#include <cfloat>

using namespace std;

struct Edge {
  int ID;
  string startCity;
  string endCity;
  bool operator<(Edge a) const {return this->ID < a.ID;}
};

struct Cost {
  int capacity;
  int cost;
};

vector<pair<Edge,Cost>> edgeList;
bool bfs(pair<string,double> start, pair<string,double> end, map<string,string> &path, unordered_map<int,double> &flowTrack);
unordered_map<int,double> organizeLogistics(vector<pair<string,double>> start, vector <pair<string,double>> end);
unordered_map<int,double> FordFulkerson(pair<string,double> start, pair<string,double> end, unordered_map<int,double> &flowTrack);

unordered_map<int,double> organizeLogistics(vector<pair<string,double>> start, vector <pair<string,double>> end) {
  // reads in all data from map file
  ifstream inFile("worldmap.txt");
  int ID;
  string startCity;
  string endCity;
  int capacity;
  int cost;

  while(!inFile.eof()) {
    inFile >> ID >> startCity >> endCity >> capacity >> cost;
    Edge newEdge = {ID, startCity, endCity};
    Cost newCost = {capacity, cost};
    edgeList.push_back(pair<Edge, Cost>(newEdge, newCost));
  }

  unordered_map<int,double> flowTrack;

  // finds optimal distribution of a single product with ford-fulkerson for each start and end
  FordFulkerson(start[0], end[0], flowTrack);

  return flowTrack;
}

//Breadth first search of the nodes in edge list to find all possible paths to end.
bool bfs(pair<string,double> start, pair<string,double> end, map<string,string> &path, unordered_map<int,double> &flowTrack) {
  set<string> visited; // Keeps track of already visited nodes.
  queue<string> queue; // Keeps track of current nodes to be processed.

  // Visit the start
  queue.push(start.first);
  visited.insert(start.first);

  // Go until no more nodes in queue
  while (!queue.empty()) {
    string current = queue.front(); // Process the front of queue
    queue.pop();

    // Goes through edge list to find all cities connected to current node
    for (auto it: edgeList) {
        double currentLeft = it.second.capacity - flowTrack.at(it.first.ID);
        // If not visited and can have flow, add it to be processed.
        if (it.first.startCity == current && visited.count(it.first.endCity) == 0 && currentLeft > 0) {
          queue.push(it.first.endCity); // add it to queue
          path.insert(pair<string, string>(it.first.endCity, it.first.startCity)); // add pair to path
          visited.insert(it.first.endCity); // add to visited
        }
    }
  }

  return (visited.count(end.first) >= 1);
}

// Finds the maximum flow through a graph
unordered_map<int,double> FordFulkerson(pair<string,double> start, pair<string,double> end, unordered_map<int,double> &flowTrack) {
  map<string,string> path; // Parent path to be added by the bfs
  double capacityAtEnd;

  for (auto x: edgeList) flowTrack.insert(pair<int, double>(x.first.ID, 0));

  // While there is still a viable path, continue.
  while (bfs(start, end, path, flowTrack)) {
    if (capacityAtEnd == start.second) break; // end if all the capacity is used up.

    double minCapacity = start.second; // sets min capacity to the amount of flow to be added
    string currentNode = end.first; // start at end

    // Search through the parent path backwards (find best path) and gets min flow
    while (currentNode != start.first) {
      string parent = path.at(currentNode); // get parent
      for (auto it: edgeList) {
        // gets min capacity in edgelist
        if (it.first.startCity == parent && it.first.endCity == currentNode){
          double currentLeft = it.second.capacity - flowTrack.at(it.first.ID);
          minCapacity = min(minCapacity, currentLeft);
          break;
        }
      }
      currentNode = parent;
    }

    // adds min flow to the corresponding
    currentNode = end.first;
    while (currentNode != start.first) {
      string parent = path.at(currentNode);
      for (auto it: edgeList) {
        double currentLeft = it.second.capacity - flowTrack.at(it.first.ID);
        if (it.first.startCity == parent && it.first.endCity == currentNode && currentLeft >= minCapacity) {
          if (it.first.endCity == end.first) capacityAtEnd += minCapacity;
          flowTrack.at(it.first.ID) += minCapacity;
          break;
        }
      }

      currentNode = parent;
    }

    // clears the path for each cycle.
    path.clear();
  }

  // Adds all flows to each
  return flowTrack;
}
