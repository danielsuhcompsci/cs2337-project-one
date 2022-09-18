// Daniel dss210005

#ifndef CREATURE_H
#define CREATURE_H

#include <string>
#include <unordered_map>

class Creature {
 protected:
  Creature(){};
  virtual std::string Move(int[4], std::unordered_map<int, char>) = 0;
  virtual std::string Breed(int[4]) = 0;
};

#endif