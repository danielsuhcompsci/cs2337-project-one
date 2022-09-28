// Daniel dss210005

#ifndef BEETLE_H
#define BEETLE_H

#include <utility>

#include "Creature.h"

class Beetle : public Creature {
 public:
  Beetle() {}
  virtual ~Beetle() {}
  char Move(int[4], const std::unordered_map<int, char>&) override;
  char Breed(bool[4], const std::unordered_map<int, char>&) override;
  bool Starve();

  // Use to parse input for move
  static void ParseInput(int[4], int[4], int[4]);

  // Resets the timer after a beetle eats an ant
  void ResetTimer();

  // Decreases the timer every turn
  void DecrementTimer();

 private:
  // Set timer at 5 initially
  int timer = 5;
};

#endif