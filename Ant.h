// Daniel dss210005

#ifndef ANT_H
#define ANT_H

#include "Creature.h"

class Ant : public Creature {
 public:
  Ant() {}
  virtual std::string Move(int[4],
                           const std::unordered_map<int, char>&) override;
  virtual std::string Breed(bool[4],
                            const std::unordered_map<int, char>&) override;
};

#endif