// Daniel dss210005

#include "Ant.h"

#include <vector>

std::string Ant::Move(int distances[4],
                      std::unordered_map<int, char> indexToDirection) {
  // Holds where the ant "wants" to go
  std::string decisionString = "";

  // If there are no beetles in all directions, return an empty decision string
  if ((distances[0] == 0) && (distances[1] == 0) && (distances[2] == 0) &&
      (distances[3] == 0))
    return decisionString;

  // The smallest nonzero distance, set at 9 in case the first element is 0.
  // Represents the closest beetle distance
  int minDistance = 9;

  // The farthest beetle distance
  int maxDistance = distances[0];

  // Amount of nearest beetles
  int minDistanceCount = 1;

  // Finds the smallest nonzero distance and the farthest distance
  for (int i = 1; i < 4; i++) {
    if ((distances[i] < minDistance) && (distances[i] > 0)) {
      minDistance = distances[i];
      minDistanceCount = 1;
    } else if (distances[i] == minDistance)
      minDistanceCount++;

    if (distances[i] > maxDistance) maxDistance = distances[i];

    // If there is only one closest beetle
    if (minDistanceCount == 1) {
      // Add the opposite direction of that beetle to the decision string and
      // return
      decisionString.push_back(indexToDirection.at((minDistance + 2) % 4));
      return decisionString;
    }

    // If there is a tie for closest beetle
    else if (minDistanceCount >= 2) {
      // Look for an empty lane in N, E, S, W order
      for (int i = 0; i < 4; i++) {
        if (distances[i] == 0) {
          decisionString.push_back(indexToDirection.at(distances[i]));
          return decisionString;
        }
      }
    }
  }
}

std::string Ant::Breed(int[4]) {}