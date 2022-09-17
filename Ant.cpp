// Daniel dss210005

#include "Ant.h"

std::string Ant::Move(int distances[4]) {  // 1 2 1 4
  std::string decisionString = "";

  int min = distances[0];  // 1
  int minCount = 1;

  for (int i = 1; i < 4; i++) {
    if (distances[i] < min) {
      min = distances[i];
      minCount = 1;
    } else if (distances[i] == min) {
      minCount++;
    }
  }
}

std::string Ant::Breed(int[4]) {}