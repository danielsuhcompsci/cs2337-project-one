// Daniel dss210005

#include <fstream>
#include <iostream>
#include <string>

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
}

void print(Creature *grid[10][10]) {
  std::cout << "enter print" << std::endl;
  for (int row = 0; row < 10; row++) {
    for (int i = 0; i < 10; i++) {
      if (i == 1) {
        std::cout << "1";
      } else {
        std::cout << "2";
      }
      if (dynamic_cast<Ant *>(grid[row][i]) != nullptr) {
        std::cout << "found ant" << std::endl;
        std::cout << "a";
      } else if (dynamic_cast<Beetle *>(grid[row][i]) != nullptr) {
        std::cout << "B";
      } else {
        std::cout << "something";
        std::cout << " ";
      }
    }

    std::cout << "end of row" << std::endl;
    std::cout << std::endl;
  }
}

int main() {
  std::string fileName, line;

  //   std::cout << "Please input the file name: " << std::endl;
  //   std::cin >> fileName;
  //   std::ifstream input(fileName);

  std::ifstream input("input.txt");

  Creature *grid[10][10];

  // Exit if file wasn't opened properly
  if (!input.is_open()) {
    std::cout << "file not opened properly" << std::endl;
    return 1;
  }

  // Read grid from file
  for (int row = 0; row < 10; row++) {
    std::getline(input, line);
    readInputRow(line, grid, row);
  }

  print(grid);
}
