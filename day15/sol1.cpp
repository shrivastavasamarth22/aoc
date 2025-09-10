#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Position {
  int row, col;
  Position(int r = 0, int c = 0) : row(r), col(c) {}
  Position operator+(const Position &other) const {
    return Position(row + other.row, col + other.col);
  }
};

class Warehouse {
private:
  vector<string> grid;
  Position robot;
  int rows, cols;

  Position getDirection(char move) {
    switch (move) {
    case '^':
      return Position(-1, 0);
    case 'v':
      return Position(1, 0);
    case '<':
      return Position(0, -1);
    case '>':
      return Position(0, 1);
    default:
      return Position(0, 0);
    }
  }

  bool isValid(const Position &pos) {
    return pos.row >= 0 && pos.row < rows && pos.col >= 0 && pos.col < cols;
  }

  char getCell(const Position &pos) {
    if (!isValid(pos))
      return '#';
    return grid[pos.row][pos.col];
  }

  void setCell(const Position &pos, char c) {
    if (isValid(pos)) {
      grid[pos.row][pos.col] = c;
    }
  }

  bool canPush(const Position &pos, const Position &dir) {
    Position next = pos + dir;
    char nextCell = getCell(next);

    if (nextCell == '#')
      return false;
    if (nextCell == '.')
      return true;
    if (nextCell == 'O')
      return canPush(next, dir);

    return false;
  }

  void pushBoxes(const Position &pos, const Position &dir) {
    Position next = pos + dir;
    char nextCell = getCell(next);

    if (nextCell == 'O') {
      pushBoxes(next, dir);
    }

    setCell(next, getCell(pos));
    setCell(pos, '.');
  }

public:
  void loadWarehouse(const string &filename) {
    ifstream file(filename);
    string line;
    bool readingMoves = false;

    while (getline(file, line)) {
      if (line.empty()) {
        readingMoves = true;
        continue;
      }

      if (!readingMoves) {
        grid.push_back(line);
        // Find robot position
        for (int col = 0; col < line.length(); col++) {
          if (line[col] == '@') {
            robot = Position(grid.size() - 1, col);
            grid.back()[col] = '.'; // Replace @ with empty space
          }
        }
      } else {
        // Process moves
        for (char move : line) {
          if (move == '^' || move == 'v' || move == '<' || move == '>') {
            makeMove(move);
          }
        }
      }
    }

    rows = grid.size();
    cols = grid[0].size();
  }

  void makeMove(char move) {
    Position dir = getDirection(move);
    Position newPos = robot + dir;
    char nextCell = getCell(newPos);

    if (nextCell == '#') {
      // Can't move into wall
      return;
    } else if (nextCell == '.') {
      // Move into empty space
      robot = newPos;
    } else if (nextCell == 'O') {
      // Try to push box(es)
      if (canPush(newPos, dir)) {
        pushBoxes(newPos, dir);
        robot = newPos;
      }
    }
  }

  void printWarehouse() {
    for (int r = 0; r < rows; r++) {
      for (int c = 0; c < cols; c++) {
        if (r == robot.row && c == robot.col) {
          cout << '@';
        } else {
          cout << grid[r][c];
        }
      }
      cout << endl;
    }
    cout << endl;
  }

  int calculateGPS() {
    int total = 0;
    for (int r = 0; r < rows; r++) {
      for (int c = 0; c < cols; c++) {
        if (grid[r][c] == 'O') {
          total += 100 * r + c;
        }
      }
    }
    return total;
  }
};

int main() {
  Warehouse warehouse;
  warehouse.loadWarehouse("input.txt");

  cout << "Final warehouse state:" << endl;
  warehouse.printWarehouse();

  int gpsSum = warehouse.calculateGPS();
  cout << "Sum of all boxes' GPS coordinates: " << gpsSum << endl;

  return 0;
}
