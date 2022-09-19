// Daniel dss210005

#include "Beetle.h"

std::string Beetle::Move(
    int distances[4], const std::unordered_map<int, char> &indexToDirection) {
  std::string decisionString = "";

  if (distances[0] == 0 && distances[1] == 0 && distances[2] == 0 &&
      distances[3] == 0) {
  }

  // The smallest nonzero distance, set at 9 in case the first element is 0
  // Represents the closest ant distance
  int minDistance = 9;

  // Amount of nearest ants
  int minDistanceCount = 1;

  // Finds the smallest nonzero distance
  for (int i = 0; i < 4; i++) {
    if ((distances[i] < minDistance) && (distances[i] > 0)) {
      minDistance = distances[i];
      minDistanceCount = 1;
    } else if (distances[i] == minDistance)
      minDistanceCount++;
  }

  // If there is one closest ant
  if (minDistanceCount == 1) {
    for (int i = 0; i < 4; i++) {
      if (distances[i] == minDistance) {
        decisionString.push_back(indexToDirection.at(i));
        return decisionString;
      }
    }
  }

  return decisionString;
}
std::string Beetle::Breed(
    bool isCreature[4], const std::unordered_map<int, char> &indexToDirection) {
  return "";
}