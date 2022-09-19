// Daniel dss210005

#ifndef CREATURE_H
#define CREATURE_H

#include <string>
#include <unordered_map>

class Creature {
 protected:
  virtual std::string Move(int[4], const std::unordered_map<int, char>&) = 0;
  virtual std::string Breed(bool[4], const std::unordered_map<int, char>&) = 0;
};

#endif