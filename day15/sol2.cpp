#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

// Represents a coordinate on the grid.
// Includes comparison operators to be usable in a std::set.
struct Position {
  int row, col;
  Position(int r = 0, int c = 0) : row(r), col(c) {}

  Position operator+(const Position &other) const {
    return Position(row + other.row, col + other.col);
  }

  bool operator<(const Position &other) const {
    if (row != other.row)
      return row < other.row;
    return col < other.col;
  }
};

class Warehouse {
private:
  vector<string> grid; // Stores the static walls ('#')
  Position robot;
  set<Position> boxes; // Stores the top-left coordinate of each '[]' box
  int rows, cols;
  string moves;

  // Converts a move character ('^', 'v', '<', '>') to a direction vector.
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

  // Checks if a position is within the grid boundaries.
  bool isValid(const Position &pos) {
    return pos.row >= 0 && pos.row < rows && pos.col >= 0 && pos.col < cols;
  }

  // Gets the character at a position (returns '#' if out of bounds).
  char getCell(const Position &pos) {
    if (!isValid(pos))
      return '#';
    return grid[pos.row][pos.col];
  }

  // Finds the top-left corner of a box entity at a given position.
  // Returns Position(-1, -1) if no box is found.
  Position getBoxAt(const Position &pos) {
    for (const auto &box_pos : boxes) {
      if (pos.row == box_pos.row &&
          (pos.col == box_pos.col || pos.col == box_pos.col + 1)) {
        return box_pos;
      }
    }
    return Position(-1, -1); // Sentinel value indicating no box found
  }

  // Recursively checks if a box and anything it pushes can move.
  // It populates the 'chain' set with all boxes that need to move.
  bool canPush(const Position &box_to_check, const Position &dir,
               set<Position> &chain) {
    Position target_pos = box_to_check + dir;
    Position target_cell_1 = target_pos;
    Position target_cell_2 = {target_pos.row, target_pos.col + 1};

    // Check for collision with walls.
    if (getCell(target_cell_1) == '#' || getCell(target_cell_2) == '#') {
      return false;
    }

    // Find any other boxes that are in the way.
    set<Position> obstructing_boxes;
    Position box1 = getBoxAt(target_cell_1);
    if (box1.row != -1) {
      obstructing_boxes.insert(box1);
    }
    Position box2 = getBoxAt(target_cell_2);
    if (box2.row != -1) {
      obstructing_boxes.insert(box2);
    }

    // For each obstructing box, check if it can also be pushed.
    for (const auto &obs_box : obstructing_boxes) {
      if (chain.count(obs_box)) {
        continue; // This box is already part of the current push chain.
      }
      chain.insert(obs_box);
      if (!canPush(obs_box, dir, chain)) {
        return false; // The chain is blocked.
      }
    }
    return true; // The push is possible.
  }

public:
  // Loads the original warehouse, transforms it, and reads the moves.
  void loadWarehouse(const string &filename) {
    ifstream file(filename);
    string line;
    vector<string> original_grid;
    bool readingMoves = false;

    while (getline(file, line)) {
      if (line.empty()) {
        readingMoves = true;
        continue;
      }
      if (!readingMoves) {
        original_grid.push_back(line);
      } else {
        moves += line;
      }
    }

    if (original_grid.empty())
      return;

    // Create the new, wider grid based on transformation rules.
    rows = original_grid.size();
    cols = original_grid[0].size() * 2;
    grid.resize(rows, string(cols, '.'));

    for (int r = 0; r < rows; ++r) {
      for (int c = 0; c < original_grid[0].size(); ++c) {
        char tile = original_grid[r][c];
        int new_c = c * 2;
        if (tile == '#') {
          grid[r][new_c] = '#';
          grid[r][new_c + 1] = '#';
        } else if (tile == 'O') {
          boxes.insert(Position(r, new_c));
        } else if (tile == '@') {
          robot = Position(r, new_c);
        }
      }
    }
  }

  // Executes a single move for the robot.
  void makeMove(char move) {
    Position dir = getDirection(move);
    Position newRobotPos = robot + dir;

    if (getCell(newRobotPos) == '#') {
      return; // Robot hits a wall.
    }

    Position box_to_push = getBoxAt(newRobotPos);

    if (box_to_push.row != -1) { // Check if a box was found
      // A box is in the way; check if it can be pushed.
      set<Position> push_chain = {box_to_push};
      if (canPush(box_to_push, dir, push_chain)) {
        // The push is possible. Move all boxes in the chain.
        vector<Position> sorted_chain(push_chain.begin(), push_chain.end());

        // Sort the boxes to move them from the back of the chain forward,
        // preventing them from overwriting each other's target space.
        if (dir.col > 0 || dir.row > 0) { // Moving right or down
          sort(sorted_chain.rbegin(), sorted_chain.rend());
        } else { // Moving left or up
          sort(sorted_chain.begin(), sorted_chain.end());
        }

        for (const auto &p : sorted_chain) {
          boxes.erase(p);
          boxes.insert(p + dir);
        }
        robot = newRobotPos; // The robot moves into the box's old space.
      }
    } else {
      // No box in the way, just move the robot.
      robot = newRobotPos;
    }
  }

  // Runs the full simulation based on the loaded move sequence.
  void runSimulation() {
    for (char move : moves) {
      if (string("^v<>").find(move) == string::npos)
        continue;
      makeMove(move);
    }
  }

  // Calculates the sum of GPS coordinates for all boxes.
  long long calculateGPS() {
    long long total = 0;
    for (const auto &box_pos : boxes) {
      total += 100LL * box_pos.row + box_pos.col;
    }
    return total;
  }
};

int main() {
  Warehouse warehouse;
  // The problem implies using the same input file as Part 1.
  // Ensure "input.txt" is in the same directory as the executable.
  warehouse.loadWarehouse("input.txt");
  warehouse.runSimulation();

  long long gpsSum = warehouse.calculateGPS();
  cout << "Sum of all boxes' final GPS coordinates: " << gpsSum << endl;

  return 0;
}
