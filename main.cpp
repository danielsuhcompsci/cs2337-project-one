// Daniel dss210005

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

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
}

void print(Creature *grid[10][10]) {
  for (int row = 0; row < 10; row++) {
    for (int i = 0; i < 10; i++) {
      if (is<Ant>(grid[row][i])) {
        std::cout << "a";
      } else if (is<Beetle>(grid[row][i])) {
        std::cout << "B";
      } else {
        std::cout << " ";
      }
    }
    std::cout << std::endl;
  }
}

int getNeighborCount(Creature *grid[10][10], const Position &position,
                     const std::unordered_map<int, char> &indexToDirection) {
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

char getFarthestDirection(
    Position position, const std::unordered_map<int, char> &indexToDirection) {
  char decision = '\0';
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

void checkOrthogonalNeighbors(
    Creature *grid[10][10], bool isCreature[4], const Position &position,
    const std::unordered_map<int, char> &indexToDirection) {
  for (int i = 0; i < 4; i++) {
    Position tempPosition(position);
    tempPosition += Position::getOffset(indexToDirection.at(i));

    if (tempPosition.isOnGrid()) {
      if (grid[tempPosition.row][tempPosition.column] != nullptr) {
        isCreature[i] = true;
      } else {
        isCreature[i] = false;
      }
    } else {
      isCreature[i] = false;
    }
  }
}

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

template <typename CreatureType>
int getNearestDistance(Creature *grid[10][10], Position position,
                       const char &direction,
                       const std::unordered_map<int, char> &indexToDirection) {
  Position tempPosition(position);

  while (tempPosition.isOnGrid()) {
    // If there is a creature of the specified type
    if (is<CreatureType>(grid[tempPosition.row][tempPosition.column])) {
      if (position.row == tempPosition.row) {
        int distance = Position::abs(tempPosition - position).column;
        if (std::is_same<CreatureType, Ant>::value) {
          distance *= 10;
          distance += getNeighborCount(grid, tempPosition, indexToDirection);
        }
        return distance;
      } else {
        int distance = Position::abs(tempPosition - position).row;
        if (std::is_same<CreatureType, Ant>::value) {
          distance *= 10;
          distance += getNeighborCount(grid, tempPosition, indexToDirection);
        }
        return distance;
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
    distances[i] = getNearestDistance<CreatureType>(
        grid, Position(column, row), indexToDirection.at(i), indexToDirection);
  }
}

template <typename CreatureType>
void movePhase(Creature *grid[10][10],
               const std::unordered_map<int, char> &indexToDirection) {
  std::vector<char> decisions = {};
  std::vector<Position> positions = {};

  for (int column = 0; column < 10; column++) {
    for (int row = 0; row < 10; row++) {
      if (is<CreatureType>(grid[row][column])) {
        CreatureType *pointer = dynamic_cast<CreatureType *>(grid[row][column]);

        int distances[4];

        // If it's a beetle, find the nearest ants
        if (std::is_same<CreatureType, Beetle>::value) {
          getNearestDistances<Ant>(grid, distances, row, column,
                                   indexToDirection);
          std::cout << "beetle" << std::endl;
        } else {  // and vice versa
          getNearestDistances<Beetle>(grid, distances, row, column,
                                      indexToDirection);
          for (int i = 0; i < 4; i++) {
            std::cout << distances[i] << " ";
          }
          std::cout << std::endl;
        }

        char decision = pointer->Move(distances, indexToDirection);

        Position currentPosition(column, row);

        if (decision == 'F') {
          decision = getFarthestDirection(currentPosition, indexToDirection);
        }

        if (!(decision == '\0')) {
          decisions.push_back(decision);
          positions.push_back(currentPosition);
        }
      }
    }
  }

  for (int i = 0; i < decisions.size(); i++) {
    moveIfPossible(grid, positions[i], decisions[i]);
  }
}

void spawnIfPossible(Creature *grid[10][10], const Position &position,
                     const char &decision) {}

void antBreedPhase(Creature *grid[10][10],
                   const std::unordered_map<int, char> &indexToDirection) {
  std::vector<char> decisions = {};
  std::vector<Position> positions = {};

  for (int column = 0; column < 10; column++) {
    for (int row = 0; row < 10; row++) {
      if (is<Ant>(grid[row][column])) {
        Ant *ptr = dynamic_cast<Ant *>(grid[row][column]);

        bool isCreature[4];

        checkOrthogonalNeighbors(grid, isCreature, Position(column, row),
                                 indexToDirection);
        const char decision = ptr->Breed(isCreature, indexToDirection);

        // If there is a decision
        if (decision != '\0') {
          Position spawnPosition(column, row);
          spawnPosition += Position::getOffset(decision);

          if (spawnPosition.isOnGrid()) {
            if (!(grid[spawnPosition.row][spawnPosition.column] == nullptr)) {
              std::cout << "spawn position not empty" << std::endl;
            }
            grid[spawnPosition.row][spawnPosition.column] = new Ant();
          }
        }
      }
    }
  }
}

void starvePhase(Creature *grid[10][10],
                 const std::unordered_map<int, char> &indexToDirection) {}

void playGame(int turns, Creature *grid[10][10],
              const std::unordered_map<int, char> &indexToDirection) {
  for (int turn = 1; turn <= turns; turn++) {
    for (int phase = 0; phase < 2; phase++) {
      switch (phase) {
        case 0:  // Beetles move
          movePhase<Beetle>(grid, indexToDirection);
          break;
        case 1:  // Ants move
          movePhase<Ant>(grid, indexToDirection);
          break;
        case 2:  // Beetles starve
          break;
      }
    }

    // Ants breed
    if (turn % 3 == 0) {
      antBreedPhase(grid, indexToDirection);
    }

    // Beetles breed
    if (turn % 8 == 0) {
    }

    std::cout << "TURN " << turn << std::endl;
    print(grid);
    std::cout << std::endl;
  }
}

int main() {
  std::string fileName, line;
  char ant, beetle;
  int turns;

  // std::cout << "Please input everything: " << std::endl;
  // std::cin >> fileName >> ant >> beetle >> turns;
  // std::ifstream input(fileName);

  std::ifstream input("input.txt");

  // Exit if file wasn't opened properly
  if (!input.is_open()) {
    std::cout << "file not opened properly" << std::endl;
    return 1;
  }

  Creature *grid[10][10];

  const std::unordered_map<int, char> indexToDirection(
      {{0, 'N'}, {1, 'E'}, {2, 'S'}, {3, 'W'}});

  // Read grid from file
  for (int row = 0; row < 10; row++) {
    std::getline(input, line);
    readInputRow(line, grid, row);
  }

  // print(grid);

  turns = 3;
  playGame(turns, grid, indexToDirection);

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
