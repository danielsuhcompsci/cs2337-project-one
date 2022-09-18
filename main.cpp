// Daniel dss210005

#include <fstream>
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
      std::cout << "Added ant"
                << " ";
    } else if (line[i] == 'B') {
      Beetle *ptr = new Beetle();
      grid[row][i] = ptr;
      std::cout << "Added beetle"
                << " ";
    } else {
      grid[row][i] = nullptr;
      std::cout << "empty"
                << " ";
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
        std::cout << " ";
      }
    }
    std::cout << std::endl;
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
  std::unordered_map<int, char> indexToDirection(
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
}
