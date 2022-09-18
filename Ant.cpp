// Daniel dss210005

#include "Ant.h"

std::string Ant::Move(
    int distances[4],
    std::unordered_map<int, char> indexToDirection) {  // 1 2 1 4

  std::string decisionString = "";

  // If there are no beetles in all directions
  if ((distances[0] == 0) && (distances[1] == 0) && (distances[2] == 0) &&
      (distances[3] == 0))
    return decisionString;

  int minDistance = distances[0];  // 1
  int minDistanceCount = 1;

  for (int i = 1; i < 4; i++) {
    if (distances[i] < minDistance) {
      minDistance = distances[i];
      minDistanceCount = 1;
    } else if (distances[i] == minDistanceCount) {
      minDistanceCount++;
    }
  }

  if (minDistanceCount == 1) {
  }
}
std::string Ant::Breed(int[4]) {}