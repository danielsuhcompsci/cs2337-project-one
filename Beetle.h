// Daniel dss210005

#ifndef BEETLE_H
#define BEETLE_H

#include <utility>

#include "Creature.h"

class Beetle : public Creature {
 public:
  Beetle() {}
  ~Beetle() {}
  virtual std::string Move(int[4],
                           const std::unordered_map<int, char>&) override;
  virtual std::string Breed(bool[4],
                            const std::unordered_map<int, char>&) override;
  bool Starve();
  void IncrementTimer();
  void DecrementTimer();
  static void ParseInput(int[4], int[4], int[4]);

 private:
  int timer = 5;
};

#endif