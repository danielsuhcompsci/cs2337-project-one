// Daniel dss210005

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>

#include "Ant.h"
#include "Beetle.h"

template <typename CreatureType>
bool is(Creature *ptr) {
  return (dynamic_cast<CreatureType *>(ptr) != nullptr) ? true : false;
}

typedef struct Position {
  int column;
  int row;

  Position(int column, int row) {
    this->column = column;
    this->row = row;
  }

  Position operator+(const Position &position) {
    return Position(this->column + position.column, this->row + position.row);
  }

  Position operator-(const Position &position) {
    return Position(this->column - position.column, this->row - position.row);
  }

  Position operator+=(const Position &position) {
    this->column += position.column;
    this->row += position.row;

    return *this;
  }

  friend std::ostream &operator<<(std::ostream &os, const Position &position) {
    return os << "Position: " << position.column << " " << position.row
              << std::endl;
  }

  bool isOnGrid() {
    return (0 <= row && row < 10 && 0 <= column && column < 10) ? true : false;
  }

  static Position abs(const Position &position) {
    return Position(std::abs(position.column), std::abs(position.row));
  }

  static Position getOffset(const char &direction) {
    switch (direction) {
      case 'N':
        return Position(0, -1);
        break;
      case 'E':
        return Position(1, 0);
        break;
      case 'S':
        return Position(0, 1);
        break;
      case 'W':
        return Position(-1, 0);
        break;
      default:
        std::cout << "Unexpected error in getOffset(char) with: " << direction
                  << std::endl;
        return Position(-1, -1);
        break;
    }
  }
} Position;

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

template <typename CreatureType>
int getNearestDistance(Creature *grid[10][10], Position position,
                       const char &direction) {
  Position tempPosition(position);

  while (tempPosition.isOnGrid()) {
    if (is<CreatureType>(grid[tempPosition.row][tempPosition.column])) {
      if (position.row == tempPosition.row) {
        return Position::abs(tempPosition - position).column;
      } else {
        return Position::abs(tempPosition - position).row;
      }
    }
    tempPosition += Position::getOffset(direction);
  }

  // There isn't a nearest creature of the specified type
  return 0;
}

template <typename CreatureType>
void getNearestDistances(
    Creature *grid[10][10], int distances[4], int row, int column,
    const std::unordered_map<int, char> &indexToDirection) {
  for (int i = 0; i < 4; i++) {
    distances[i] = getNearestDistance<CreatureType>(grid, Position(column, row),
                                                    indexToDirection.at(i));
  }
}

char getFarthestDirection(
    Position position, const std::unordered_map<int, char> &indexToDirection) {
  char decision;
  int distances[4];

  distances[0] = position.row;         // North
  distances[1] = 9 - position.column;  // East
  distances[2] = 9 - position.row;     // South
  distances[3] = position.column;      // West

  // Find the max distance
  int max = distances[0];
  for (int i = 1; i < 4; i++) {
    if (distances[i] > max) {
      max = distances[i];
    }
  }

  // Returns the direction of the max distance in N, E, S, W order
  for (int i = 0; i < 4; i++) {
    if (distances[i] == max) {
      decision = indexToDirection.at(i);
      return decision;
    }
  }

  // Function should not go past this point because max must be a number
  // within distances[4]
  std::cout << "Unexpected error in getFarthestDirection()" << std::endl;

  return decision;
}

int getNeighborCount(Creature *grid[10][10], const Position &position,
                     const std::unordered_map<char, int> &indexToDirection) {
  int count = 0;

  // N, E, S, W | N NE, E SE, S SW, W NW
  for (int i = 0; i < 4; i++) {
    // Create mutable position temp struct
    Position tempPosition(position);

    // Add corresponding orthogonal offset
    // For example if (i = 0) it checks the North neighbor
    tempPosition += Position::getOffset(indexToDirection.at(i));

    if (tempPosition.isOnGrid()) {
      if (is<Ant>(grid[tempPosition.row][tempPosition.column])) {
        count++;
      }
    }

    // Make offset diagonal
    // For example if (i = 0) it checks the Northeast neighbor (by adding the
    // east offset)
    tempPosition += Position::getOffset(indexToDirection.at((i + 1) % 4));

    if (tempPosition.isOnGrid()) {
      if (is<Ant>(grid[tempPosition.row][tempPosition.column])) {
        count++;
      }
    }
  }

  return count;
}

// void checkOrthogonalNeighbors(Creature *grid[10][10], bool isCreature[4],
//                               int row, int column) {}

void moveIfPossible(Creature *grid[10][10], const Position &position,
                    const char &decision) {
  Position newPosition(position);
  newPosition += Position::getOffset(decision);

  // Make sure it's on the grid
  if (newPosition.isOnGrid()) {
    // If its empty
    if (grid[newPosition.row][newPosition.column] == nullptr) {
      grid[newPosition.row][newPosition.column] =
          grid[position.row][position.column];
      grid[position.row][position.column] = nullptr;
    } else {  // If there is a creature
      // If a beetle is eating an ant
      if (is<Beetle>(grid[position.row][position.column]) &&
          is<Ant>(grid[newPosition.row][newPosition.column])) {
        // Deallocate ant
        delete grid[newPosition.row][newPosition.column];

        // Move beetle to where ant was
        grid[newPosition.row][newPosition.column] =
            grid[position.row][position.column];

        // Set beetles old spot to empty
        grid[position.row][position.column] = nullptr;
      }
    }
  }
}

void beetlePhase(Creature *grid[10][10],
                 const std::unordered_map<int, char> &indexToDirection) {
  for (int column = 0; column < 10; column++) {
    for (int row = 0; row < 10; row++) {
      if (is<Beetle>(grid[row][column])) {
        Beetle *beetlePointer = dynamic_cast<Beetle *>(grid[row][column]);

        int distances[4];

        getNearestDistances<Ant>(grid, distances, row, column,
                                 indexToDirection);

        const char decision = beetlePointer->Move(distances, indexToDirection);

        moveIfPossible(grid, Position(column, row), decision);
      }
    }
  }
}

void antPhase(Creature *grid[10][10],
              const std::unordered_map<int, char> &indexToDirection) {}

void starvePhase(Creature *grid[10][10],
                 const std::unordered_map<int, char> &indexToDirection) {}

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

  const std::unordered_map<char, Position> directionToOffset();

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
