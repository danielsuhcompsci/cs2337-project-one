// Daniel dss210005

#ifndef BEETLE_H
#define BEETLE_H

#include <utility>

#include "Creature.h"

class Beetle : public Creature {
 public:
  Beetle() {}
  char Move(int[4], const std::unordered_map<int, char>&) override;
  char Breed(bool[4], const std::unordered_map<int, char>&) override;
  bool Starve();
  void ResetTimer();
  void DecrementTimer();
  static void ParseInput(int[4], int[4], int[4]);

 private:
  int timer = 5;
};

#endif