// Daniel dss210005

#include "Ant.h"

#include <iostream>

char Ant::Move(int distances[4],
               const std::unordered_map<int, char> &indexToDirection) {
  // If there are no beetles in all directions, return an empty decision string
  if ((distances[0] == 0) && (distances[1] == 0) && (distances[2] == 0) &&
      (distances[3] == 0))
    return '\0';

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

  // If there is only one closest beetle
  if (minDistanceCount == 1) {
    // Find the index for the closest distance
    int minDistanceIndex = 0;
    while (distances[minDistanceIndex] != minDistance &&
           (minDistanceIndex < 4)) {
      minDistanceIndex++;
    }

    // Add the opposite direction to the decision string
    return indexToDirection.at((minDistanceIndex + 2) % 4);
  }

  // If there is a tie for closest beetle
  else if (minDistanceCount == 2 || minDistanceCount == 3) {
    // Look for an empty lane in N, E, S, W order
    for (int i = 0; i < 4; i++) {
      if (distances[i] == 0) {
        return indexToDirection.at(i);
      }
    }

    // Find the farthest distance
    for (int i = 0; i < 4; i++) {
      if (distances[i] == maxDistance) {
        return indexToDirection.at(i);
      }
    }
  }

  // Should not reach this point
  return '\0';
}

char Ant::Breed(bool isEmpty[4],
                const std::unordered_map<int, char> &indexToDirection) {
  // isEmpty holds booleans for valid spawn positions in N, E, S, W order
  // If there is an empty spot, return the decision
  for (int i = 0; i < 4; i++) {
    if (isEmpty[i]) {
      return indexToDirection.at(i);
    }
  }

  // Else return a null decision
  return '\0';
}