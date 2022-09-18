// Daniel dss210005

#ifndef BEETLE_H
#define BEETLE_H

#include "Creature.h"

class Beetle : public Creature {
 public:
  Beetle() {}
  virtual std::string Move(int[4], std::unordered_map<int, char>) override;
  virtual std::string Breed(int[4]) override;
};

#endif