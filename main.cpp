// Daniel dss210005

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>

#include "Ant.h"
#include "Beetle.h"

void readInputRow(std::string &line, Creature *grid[10][10], int row) {
  for (int i = 0; i < line.length(); i++) {
    if (line[i] == 'a') {
      Ant *ptr = new Ant();
      grid[row][i] = ptr;
    } else if (line[i] == 'B') {
      Beetle *ptr = new Beetle();
      grid[row][i] = ptr;
    } else {
      grid[row][i] = nullptr;
    }
  }
  std::cout << std::endl;
}

void print(Creature *grid[10][10]) {
  for (int row = 0; row < 10; row++) {
    for (int i = 0; i < 10; i++) {
      if (dynamic_cast<Ant *>(grid[row][i]) != nullptr) {
        std::cout << "a";
      } else if (dynamic_cast<Beetle *>(grid[row][i]) != nullptr) {
        std::cout << "B";
      } else {
        std::cout << ".";
      }
    }
    std::cout << std::endl;
  }
}

void playGame(int turns, Creature *grid[10][10]) {
  for (int turn = 1; turn <= turns; turn++) {
    for (int column = 0; column < 10; column++) {
      for (int row = 0; row < 10; row++) {
      }
    }
  }
}

int main() {
  std::string fileName, line;

  //   std::cout << "Please input the file name: " << std::endl;
  //   std::cin >> fileName;
  //   std::ifstream input(fileName);

  std::ifstream input("input.txt");

  // Exit if file wasn't opened properly
  if (!input.is_open()) {
    std::cout << "file not opened properly" << std::endl;
    return 1;
  }

  Creature *grid[10][10];
  const std::unordered_map<int, char> indexToDirection(
      {{0, 'N'}, {1, 'E'}, {2, 'S'}, {3, 'W'}});

  // getline(input, line);
  // std::cout << line << std::endl;

  // Read grid from file
  for (int row = 0; row < 10; row++) {
    std::getline(input, line);
    // std::cout << line;
    readInputRow(line, grid, row);
  }

  print(grid);

  // Ant a;
  // int distances[4] = {1, 1, 0, 1};  // N, E, S, W
  // bool isCreature[4] = {true, false, true, false};
  // std::cout << "Ant move: " << a.Move(distances, indexToDirection) <<
  // std::endl; std::cout << "Ant breed: " << a.Breed(isCreature,
  // indexToDirection)
  //           << std::endl;

  // int beetleDistances[4] = {11, 11, 11, 11};  // N, E, S, W
  // Beetle b;
  // std::cout << "Beetle move: " << b.Move(beetleDistances, indexToDirection)
  //           << std::endl;
  // std::cout << "Beetle breed: " << b.Breed(isCreature, indexToDirection)
  //           << std::endl;
}
