// Daniel dss210005

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "Ant.h"
#include "Beetle.h"

// Checks if the pointer is of the specified creature type by dynamic casting
template <typename CreatureType>
bool is(Creature *ptr) {
  return (dynamic_cast<CreatureType *>(ptr) != nullptr) ? true : false;
}

// Used for a position on a grid and to hold related operators and functionality
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

  bool operator==(const Position &position) {
    return (this->column == position.column) && (this->row == position.row);
  }

  friend std::ostream &operator<<(std::ostream &os, const Position &position) {
    return os << "Position: " << position.column << " " << position.row
              << std::endl;
  }

  // Checks if the position within the grid boundaries
  bool isOnGrid() {
    return (0 <= row && row < 10 && 0 <= column && column < 10) ? true : false;
  }

  // Gets the absolute value of the position
  static Position abs(const Position &position) {
    return Position(std::abs(position.column), std::abs(position.row));
  }

  // Returns the appropriate unit offset for a specified direction,
  // assuming: +X direction is East, and +Y direction is South
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

// Reads in the input according to format, populates the grid pointer array
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

// Prints the grid according to the specified format
void print(Creature *grid[10][10], std::string ant, std::string beetle) {
  for (int row = 0; row < 10; row++) {
    for (int i = 0; i < 10; i++) {
      if (is<Ant>(grid[row][i])) {
        std::cout << ant;
      } else if (is<Beetle>(grid[row][i])) {
        std::cout << beetle;
      } else {
        std::cout << " ";
      }
    }
    std::cout << std::endl;
  }
}

// Returns the number of adjacent orthogonal and diagonal neighbors relative to
// a given position
int getNeighborCount(Creature *grid[10][10], const Position &position,
                     const std::unordered_map<int, char> &indexToDirection) {
  // Holds the neighbor count
  int count = 0;

  // Order of checking:
  // N, E, S, W | N NE, E SE, S SW, W NW
  for (int i = 0; i < 4; i++) {
    // Create mutable position temp struct
    Position tempPosition(position);

    // Add corresponding orthogonal offset
    // For example if (i = 0) it checks the North neighbor
    tempPosition += Position::getOffset(indexToDirection.at(i));

    // If the orthogonal position is on the grid and it has an ant, increment
    // count
    if (tempPosition.isOnGrid()) {
      if (is<Ant>(grid[tempPosition.row][tempPosition.column])) {
        count++;
      }
    }

    // Make offset diagonal
    // For example if (i = 0) it checks the Northeast neighbor (by adding the
    // east offset)
    tempPosition += Position::getOffset(indexToDirection.at((i + 1) % 4));

    // If the diagonal position is on the grid and it has an ant, increment
    // count
    if (tempPosition.isOnGrid()) {
      if (is<Ant>(grid[tempPosition.row][tempPosition.column])) {
        count++;
      }
    }
  }

  return count;
}

// Gets the farthest direction to move to from a specifed position, used with
// Beetle::Move()
char getFarthestDirection(
    Position position, const std::unordered_map<int, char> &indexToDirection) {
  // Decision, initially set to null char
  char decision = '\0';
  // Holds the distances to grid boundary in every direction
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

  // Returns the decision corresponding to the max distance in N, E, S, W order
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

// Checks orthogonal directions for valid breed positions, used with
// Ant::Breed() and Beetle::Breed()
void checkOrthogonalNeighbors(
    Creature *grid[10][10], bool isEmpty[4], const Position &position,
    const std::unordered_map<int, char> &indexToDirection) {
  // Iterate through orthogonal directions
  for (int i = 0; i < 4; i++) {
    // Temporary position for each offset: N, E, S, W
    Position tempPosition(position);
    tempPosition += Position::getOffset(indexToDirection.at(i));

    // If this position is on the grid
    if (tempPosition.isOnGrid()) {
      // If there is a creature, set bool to false at the correct position in
      // isEmpty
      if (grid[tempPosition.row][tempPosition.column]) {
        isEmpty[i] = false;
      } else {  // Else, set bool to true at the correct position in isEmpty
        isEmpty[i] = true;
      }
    } else {  // Else (it's not on the grid), set current direction bool to
              // false
      isEmpty[i] = false;
    }
  }
}

// Performs a move given a position and a decision, returns true if the move
// happened, false otherwise
bool moveIfPossible(Creature *grid[10][10], const Position &position,
                    const char &decision) {
  // The position to be moved to
  Position newPosition(position);
  newPosition += Position::getOffset(decision);

  // Make sure it's on the grid
  if (newPosition.isOnGrid()) {
    // If its empty, move creature from old position to new position
    if (grid[newPosition.row][newPosition.column] == nullptr) {
      grid[newPosition.row][newPosition.column] =
          grid[position.row][position.column];
      grid[position.row][position.column] = nullptr;

      return true;
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

        // Reset beetle starving timer
        Beetle *ptr =
            dynamic_cast<Beetle *>(grid[newPosition.row][newPosition.column]);
        ptr->ResetTimer();

        return true;
      }

      // Return false because any other creature-onto-creature interaction
      // doesn't work
      return false;
    }
  }

  // Return false if it's not on the grid
  return false;
}

// Gets the distance to the nearest creature of the specified type, in the
// specified direction
template <typename CreatureType>
int getNearestDistance(Creature *grid[10][10], Position position,
                       const char &direction,
                       const std::unordered_map<int, char> &indexToDirection) {
  // Temporary position used to locate the position of the nearest creature of
  // the specified type
  Position tempPosition(position);

  // While the temporary position is on the grid
  while (tempPosition.isOnGrid()) {
    // If there is a creature of the specified type
    if (is<CreatureType>(grid[tempPosition.row][tempPosition.column])) {
      // If the positions are on the same x-axis,
      if (position.row == tempPosition.row) {
        // Get the distance between original and temp position
        int distance = Position::abs(tempPosition - position).column;

        // If it's a beetle looking for the nearest ant, change the formatting
        // and add the neighbor count accordingly
        if (std::is_same<CreatureType, Ant>::value) {
          distance *= 10;
          distance += getNeighborCount(grid, tempPosition, indexToDirection);
        }
        return distance;
      } else {  // Else the positions are on the same y-axis
        // Get the distance between the original and temp position
        int distance = Position::abs(tempPosition - position).row;

        // If it's a beetle looking for the nearest ant, change the formatting
        // and add the neighbor count accordingly
        if (std::is_same<CreatureType, Ant>::value) {
          distance *= 10;
          distance += getNeighborCount(grid, tempPosition, indexToDirection);
        }
        return distance;
      }
    }
    // Increment the position in the right direction
    tempPosition += Position::getOffset(direction);
  }

  // There isn't a nearest creature of the specified type
  return 0;
}

// Gets the nearest distances to a specified creature type
template <typename CreatureType>
void getNearestDistances(
    Creature *grid[10][10], int distances[4], int row, int column,
    const std::unordered_map<int, char> &indexToDirection) {
  // Iterate through orthogonal directions
  for (int i = 0; i < 4; i++) {
    distances[i] = getNearestDistance<CreatureType>(
        grid, Position(column, row), indexToDirection.at(i), indexToDirection);
  }
}

// Holds the logic for the move phase of a turn, for a specified creature type
template <typename CreatureType>
void movePhase(Creature *grid[10][10],
               const std::unordered_map<int, char> &indexToDirection) {
  // Holds the positions that have been moved to this phase, makes sure a
  // creature doesn't move twice in the same phase
  std::vector<Position> positionsMovedTo = {};

  // Iterate through the board for each phase, column first
  for (int column = 0; column < 10; column++) {
    for (int row = 0; row < 10; row++) {
      // If it's the specified creature type
      if (is<CreatureType>(grid[row][column])) {
        // Get the pointer of the correct type to the creature
        CreatureType *pointer = dynamic_cast<CreatureType *>(grid[row][column]);

        // Holds the nearest distances from the creature
        int distances[4];

        // If it's a beetle, find the nearest ants
        if (std::is_same<CreatureType, Beetle>::value) {
          getNearestDistances<Ant>(grid, distances, row, column,
                                   indexToDirection);
        } else {  // And vice versa: if it's an ant, find the nearest beetles
          getNearestDistances<Beetle>(grid, distances, row, column,
                                      indexToDirection);
        }

        // Make a decision based on the distances
        char decision = pointer->Move(distances, indexToDirection);
        // Current position on grid
        Position currentPosition(column, row);

        // Beetle::Move() can return 'F'
        // If it does, we update the direction of the decision to be in the
        // farthest direction
        if (decision == 'F') {
          decision = getFarthestDirection(currentPosition, indexToDirection);
        }

        // If there is a decision and it's not occuring from a position that has
        // already been moved to
        if (!(decision == '\0') &&
            (std::find(positionsMovedTo.begin(), positionsMovedTo.end(),
                       currentPosition) == positionsMovedTo.end())) {
          // Apply decision if possible
          bool didMove = moveIfPossible(grid, currentPosition, decision);

          // If it moved, add it to the list of positions moved to
          if (didMove) {
            positionsMovedTo.push_back(currentPosition +
                                       Position::getOffset(decision));
          }
        }
      }
    }
  }
}

// Holds the logic for the breed phase of a turn, for a specified creature type
template <typename CreatureType>
void breedPhase(Creature *grid[10][10],
                const std::unordered_map<int, char> &indexToDirection) {
  // Holds the positions of creatures that were spawned this phase, so that a
  // spawned creature doesn't try to breed
  std::vector<Position> positionsSpawnedOn = {};

  // Iterate through the grid
  for (int column = 0; column < 10; column++) {
    for (int row = 0; row < 10; row++) {
      // If there is a creature of the specified type
      if (is<CreatureType>(grid[row][column])) {
        CreatureType *ptr = dynamic_cast<CreatureType *>(grid[row][column]);

        // Check for orthogonal neighbors and pass into Ant::Breed()
        bool isEmpty[4];
        checkOrthogonalNeighbors(grid, isEmpty, Position(column, row),
                                 indexToDirection);
        const char decision = ptr->Breed(isEmpty, indexToDirection);
        Position currentPosition(column, row);

        // If there is a decision and the current creature isn't a spawned
        // creature
        if (!(decision == '\0') &&
            (std::find(positionsSpawnedOn.begin(), positionsSpawnedOn.end(),
                       currentPosition)) == positionsSpawnedOn.end()) {
          // Apply decision if possible
          Position spawnPosition =
              currentPosition + Position::getOffset(decision);

          assert(spawnPosition.isOnGrid());
          assert(grid[spawnPosition.row][spawnPosition.column] == nullptr);

          // Add the new creature to the grid
          if (std::is_same<CreatureType, Beetle>::value) {
            grid[spawnPosition.row][spawnPosition.column] = new Beetle();
          } else {
            grid[spawnPosition.row][spawnPosition.column] = new Ant();
          }

          // Add the new position to the vector of positions spawned on
          positionsSpawnedOn.push_back(spawnPosition);
        }
      }
    }
  }
}

// Beetle starve phase
void starvePhase(Creature *grid[10][10],
                 const std::unordered_map<int, char> &indexToDirection) {
  // iterate through grid
  for (int column = 0; column < 10; column++) {
    for (int row = 0; row < 10; row++) {
      // If there is a beetle
      if (is<Beetle>(grid[row][column])) {
        Beetle *ptr = dynamic_cast<Beetle *>(grid[row][column]);
        // If the beetle starved, deallocate it and set the grid position to
        // null
        if (ptr->Starve()) {
          delete ptr;
          grid[row][column] = nullptr;
        }
      }
    }
  }
}

// Holds the logic for the gameplay for the specified number of turns
void playGame(int turns, Creature *grid[10][10],
              const std::unordered_map<int, char> &indexToDirection,
              std::string ant, std::string beetle) {
  for (int turn = 1; turn <= turns; turn++) {
    // Beetles Move Phase
    movePhase<Beetle>(grid, indexToDirection);

    // std::cout << "AFTER BEETLE MOVE" << std::endl;
    // print(grid);
    // std::cout << std::endl;

    // Ants Move Phase
    movePhase<Ant>(grid, indexToDirection);

    // std::cout << "AFTER ANT MOVE" << std::endl;
    // print(grid);
    // std::cout << std::endl;

    // Beetle starve timer decrement every turn
    for (int column = 0; column < 10; column++) {
      for (int row = 0; row < 10; row++) {
        if (is<Beetle>(grid[row][column])) {
          Beetle *ptr = dynamic_cast<Beetle *>(grid[row][column]);
          ptr->DecrementTimer();
        }
      }
    }

    // Beetles Starve Phase
    if (turn >= 5) {
      starvePhase(grid, indexToDirection);
    }
    // Ants Breed Phase
    if (turn % 3 == 0) {
      breedPhase<Ant>(grid, indexToDirection);
    }

    // Beetles Breed Phase
    if (turn % 8 == 0) {
      breedPhase<Beetle>(grid, indexToDirection);
    }

    // Print the turn state
    std::cout << "TURN " << turn << std::endl;
    print(grid, ant, beetle);
    std::cout << std::endl;
  }
}

int main() {
  std::string fileName, line, ant, beetle;
  int turns;

  // std::cout << "Please input everything: " << std::endl;
  std::cin >> fileName >> ant >> beetle >> turns;
  // std::cin >> fileName;
  std::ifstream input(fileName);

  // std::ifstream input("input.txt");

  // Exit if file wasn't opened properly
  if (!input.is_open()) {
    std::cout << "file not opened properly" << std::endl;
    return 1;
  }

  // Array of creature pointers
  Creature *grid[10][10];

  // Holds the map to from an index to a direction character
  const std::unordered_map<int, char> indexToDirection(
      {{0, 'N'}, {1, 'E'}, {2, 'S'}, {3, 'W'}});

  // Read grid from file
  for (int row = 0; row < 10; row++) {
    std::getline(input, line);
    readInputRow(line, grid, row);
  }

  // Print out the initial state
  std::cout << "INITIAL GRID" << std::endl;
  print(grid, ant, beetle);

  // Initiates game
  playGame(turns, grid, indexToDirection, ant, beetle);

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
