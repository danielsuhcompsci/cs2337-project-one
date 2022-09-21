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
      if (is<Ant>(grid[row][i])) {
        std::cout << "a";
      } else if (is<Beetle>(grid[row][i])) {
        std::cout << "B";
      } else {
        std::cout << ".";
      }
    }
    std::cout << std::endl;
  }
}

bool isOnGrid(int row, int column) {
  return (0 <= row && row < 10 && 0 <= column && column < 10) ? true : false;
}

template <typename CreatureType>
bool is(Creature *ptr) {
  return (dynamic_cast<CreatureType *>(ptr) != nullptr) ? true : false;
}

template <typename CreatureType>
int getNearestDistance(int &directionIndex) {
  switch (directionIndex) {
    case 0:  // North (-y direction)
      rowIndex--;

      // Iterate up until the specified creature is found
      while (isOnGrid(rowIndex, column)) {
        if (is<CreatureType>(grid[rowIndex][column])) {
          return row - rowIndex;
        }
        rowIndex--;
      }

      // Else return 0
      return 0;
      break;
    case 1:  // East (+x direction)
      columnIndex++;

      while (isOnGrid(row, columnIndex)) {
        if (is<CreatureType>(grid[row][columnIndex])) {
          return columnIndex - column;
        }
        columnIndex++;
      }

      return 0;
      break;
    case 2:
      rowIndex++;

      while (isOnGrid(rowIndex, column)) {
        if (is<CreatureType>(grid[rowIndex][column])) {
          return rowIndex - row;
        }
        rowIndex++;
      }

      return 0;
      break;
    case 3:
      columnIndex--;

      while (isOnGrid(rowIndex, column)) {
        if (is<CreatureType>(grid[rowIndex][column])) {
          return column - columnIndex;
        }
        columnIndex--;
      }

      return 0;
      break;
    default:
      std::cout << "Unexpected error in getNearestDistances()" << std::endl;

      return 0;
      break;
  }
}

template <typename CreatureType>
void getNearestDistances(
    Creature *grid[10][10], int distances[4], int row, int column,
    const std::unordered_map<int, char> &indexToDirection) {
  // Row and columns indexes used to iterate through grid
  int rowIndex = row, columnIndex = column;

  // Iterate through each direction and set the starting
  for (int directionIndex = 0; directionIndex < 4; directionIndex++) {
    // Reset the index to point to original position
    rowIndex = row;
    columnIndex = column;
  }

  // North direction (-y direction)
  rowIndex = row - 1;

  while (isOnGrid(rowIndex, column)) {
    if (is<CreatureType>(grid[rowIndex][column])) {
      distances[0] = row - rowIndex;
    }
    rowIndex--;
  }

  for (int rowIndex = row - 1; rowIndex >= 0; rowIndex--) {
  }
}

void getFarthestDistance(Creature *grid[10][10]) {}

int getNeighborCount(Creature *grid[10][10], int row, int column) { return 0; }

void getBreedInfo(Creature *grid[10][10], bool isCreature[4], int row,
                  int column) {}

void beetlePhase(Creature *grid[10][10],
                 const std::unordered_map<int, char> &indexToDirection) {
  for (int column = 0; column < 10; column++) {
    for (int row = 0; row < 10; row++) {
      if (is<Beetle>(grid[row][column])) {
        Beetle *beetlePointer = dynamic_cast<Beetle *>(grid[row][column]);

        int distances[4];

        std::string decisionString =
            beetlePointer->Move(distances, indexToDirection);
      }
    }
  }

  void playGame(int turns, Creature *grid[10][10],
                const std::unordered_map<int, char> &indexToDirection) {
    for (int turn = 1; turn <= turns; turn++) {
      for (int phase = 0; phase < 2; phase++) {
        switch (phase) {
          case 0:  // Beetles move
            break;
          case 1:  // Ants move
            break;
          case 2:  // Beetles starve
            break;
        }
      }

      // Ants breed
      if (turn % 3 == 0) {
      }

      // Beetles breed
      if (turn % 8 == 0) {
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
    // std::cout << "Beetle move: " << b.Move(beetleDistances,
    // indexToDirection)
    //           << std::endl;
    // std::cout << "Beetle breed: " << b.Breed(isCreature, indexToDirection)
    //           << std::endl;
  }
