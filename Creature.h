// Daniel dss210005

#ifndef CREATURE_H
#define CREATURE_H

#include <string>

class Creature {
 protected:
  virtual std::string Move(int[4]) = 0;
  virtual std::string Breed(int[4]) = 0;
};

#endif