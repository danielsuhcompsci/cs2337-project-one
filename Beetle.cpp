// Daniel dss210005

#include "Beetle.h"

#include <iostream>

void Beetle::ParseInput(int input[4], int distances[4], int neighbors[4]) {
  for (int i = 0; i < 4; i++) {
    distances[i] = input[i] / 10;  // First digit
    neighbors[i] = input[i] % 10;  // Second digit
  }
}

char Beetle::Move(int input[4],
                  const std::unordered_map<int, char> &indexToDirection) {
  int distances[4];
  int neighbors[4];

  Beetle::ParseInput(input, distances, neighbors);

  // If there are no ants in any direction, return 'F' standing for farthest
  // edge which tells main to find the farthest edge
  if (distances[0] == 0 && distances[1] == 0 && distances[2] == 0 &&
      distances[3] == 0) {
    return 'F';
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
    // Find the index of the closest ant and return the corresponding direction
    // character
    for (int i = 0; i < 4; i++) {
      if (distances[i] == minDistance) {
        return indexToDirection.at(i);
      }
    }
  }

  // If there is a tie, find which nearest ant has the most neighbors
  if (minDistanceCount >= 2) {
    // The most neighbors out of nearest ants
    int mostNeighbors = neighbors[0];

    // Finds the highest neighbor amount out of the closest ants
    for (int i = 0; i < 4; i++) {
      if (distances[i] == minDistance && neighbors[i] > mostNeighbors) {
        mostNeighbors = neighbors[i];
      }
    }

    // Return the decision for the ant with the most neighbors in N, E, S, W
    // order
    for (int i = 0; i < 4; i++) {
      if (distances[i] == minDistance && neighbors[i] == mostNeighbors) {
        return indexToDirection.at(i);
      }
    }
  }

  // Should not reach this point
  return '\0';
}

char Beetle::Breed(bool isCreature[4],
                   const std::unordered_map<int, char> &indexToDirection) {
  for (int i = 0; i < 4; i++) {
    if (isCreature[i] == true) {
      return indexToDirection.at(i);
    }
  }

  return '\0';
}

bool Beetle::Starve() { return (timer == 0) ? true : false; }

void Beetle::ResetTimer() { this->timer = 5; }
void Beetle::DecrementTimer() { this->timer--; }