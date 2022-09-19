// Daniel dss210005

#include "Ant.h"

#include <iostream>

std::string Ant::Move(int distances[4],
                      const std::unordered_map<int, char> &indexToDirection) {
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
  for (int i = 0; i < 4; i++) {
    if ((distances[i] < minDistance) && (distances[i] > 0)) {
      minDistance = distances[i];
      minDistanceCount = 1;
    } else if (distances[i] == minDistance)
      minDistanceCount++;

    if (distances[i] > maxDistance) maxDistance = distances[i];
  }

  std::cout << "min: " << minDistance << std::endl;
  std::cout << "max: " << maxDistance << std::endl;
  std::cout << "min count: " << minDistanceCount << std::endl;

  // If there is only one closest beetle
  if (minDistanceCount == 1) {
    // Find the index for the closest distance
    int minDistanceIndex = 0;
    while (distances[minDistanceIndex] != minDistance &&
           (minDistanceIndex < 4)) {
      minDistanceIndex++;
    }

    // Add the opposite direction to the decision string
    decisionString.push_back(indexToDirection.at((minDistanceIndex + 2) % 4));
    return decisionString;
  }

  // If there is a tie for closest beetle
  else if (minDistanceCount == 2 || minDistanceCount == 3) {
    // Look for an empty lane in N, E, S, W order
    for (int i = 0; i < 4; i++) {
      if (distances[i] == 0) {
        decisionString.push_back(indexToDirection.at(i));
        return decisionString;
      }
    }

    // Find the farthest distance
    for (int i = 0; i < 4; i++) {
      if (distances[i] == maxDistance) {
        decisionString.push_back(indexToDirection.at(i));
        return decisionString;
      }
    }
  }

  // If the ant is equally surrounded there is nowhere to move

  return decisionString;
}

std::string Ant::Breed(bool isCreature[4],
                       const std::unordered_map<int, char> &indexToDirection) {
  std::string decisionString = "";

  for (int i = 0; i < 4; i++) {
    if (isCreature[i] == true) {
      decisionString.push_back(indexToDirection.at(i));
      return decisionString;
    }
  }

  return decisionString;
}