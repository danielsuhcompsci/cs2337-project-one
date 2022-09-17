// Daniel dss210005

#ifndef ANT_H
#define ANT_H

#include "Creature.h"

class Ant : public Creature {
 public:
  Ant() {}
  virtual std::string Move(int[4]);
  virtual std::string Breed(int[4]);
};

#endif