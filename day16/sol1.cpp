#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

using namespace std;

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
  bool operator==(const Position &other) const {
    return row == other.row && col == other.col;
  }
};

struct State {
  Position pos;
  int dir; // 0=East, 1=South, 2=West, 3=North
  int cost;

  State(Position p, int d, int c) : pos(p), dir(d), cost(c) {}

  bool operator>(const State &other) const { return cost > other.cost; }
};

struct StateKey {
  Position pos;
  int dir;

  StateKey(Position p, int d) : pos(p), dir(d) {}

  bool operator<(const StateKey &other) const {
    if (pos < other.pos)
      return true;
    if (other.pos < pos)
      return false;
    return dir < other.dir;
  }
};

class ReindeerMaze {
private:
  vector<string> grid;
  Position start, end;
  int rows, cols;

  // Directions: East, South, West, North
  vector<Position> directions = {
      Position(0, 1),  // East
      Position(1, 0),  // South
      Position(0, -1), // West
      Position(-1, 0)  // North
  };

  bool isValid(const Position &pos) {
    return pos.row >= 0 && pos.row < rows && pos.col >= 0 && pos.col < cols &&
           grid[pos.row][pos.col] != '#';
  }

public:
  void loadMaze(const string &filename) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
      grid.push_back(line);

      // Find start and end positions
      for (int col = 0; col < line.length(); col++) {
        if (line[col] == 'S') {
          start = Position(grid.size() - 1, col);
        } else if (line[col] == 'E') {
          end = Position(grid.size() - 1, col);
        }
      }
    }

    rows = grid.size();
    cols = grid.empty() ? 0 : grid[0].size();
  }

  int findMinimumScore() {
    priority_queue<State, vector<State>, greater<State>> pq;
    map<StateKey, int> visited;

    // Start facing East (direction 0)
    pq.push(State(start, 0, 0));

    while (!pq.empty()) {
      State current = pq.top();
      pq.pop();

      StateKey key(current.pos, current.dir);

      // If we've seen this state with a better cost, skip
      if (visited.count(key) && visited[key] <= current.cost) {
        continue;
      }
      visited[key] = current.cost;

      // If we reached the end, return the cost
      if (current.pos == end) {
        return current.cost;
      }

      // Try moving forward
      Position nextPos = current.pos + directions[current.dir];
      if (isValid(nextPos)) {
        State nextState(nextPos, current.dir, current.cost + 1);
        StateKey nextKey(nextPos, current.dir);

        if (!visited.count(nextKey) || visited[nextKey] > nextState.cost) {
          pq.push(nextState);
        }
      }

      // Try turning clockwise
      int clockwiseDir = (current.dir + 1) % 4;
      State clockwiseState(current.pos, clockwiseDir, current.cost + 1000);
      StateKey clockwiseKey(current.pos, clockwiseDir);

      if (!visited.count(clockwiseKey) ||
          visited[clockwiseKey] > clockwiseState.cost) {
        pq.push(clockwiseState);
      }

      // Try turning counterclockwise
      int counterclockwiseDir = (current.dir + 3) % 4; // +3 is same as -1 mod 4
      State counterclockwiseState(current.pos, counterclockwiseDir,
                                  current.cost + 1000);
      StateKey counterclockwiseKey(current.pos, counterclockwiseDir);

      if (!visited.count(counterclockwiseKey) ||
          visited[counterclockwiseKey] > counterclockwiseState.cost) {
        pq.push(counterclockwiseState);
      }
    }

    return -1; // No path found
  }

  void printMaze() {
    cout << "Maze (" << rows << "x" << cols << "):" << endl;
    for (const string &row : grid) {
      cout << row << endl;
    }
    cout << "Start: (" << start.row << ", " << start.col << ")" << endl;
    cout << "End: (" << end.row << ", " << end.col << ")" << endl;
    cout << endl;
  }
};

int main() {
  ReindeerMaze maze;
  maze.loadMaze("input.txt");

  maze.printMaze();

  int minScore = maze.findMinimumScore();

  if (minScore != -1) {
    cout << "Minimum score: " << minScore << endl;
  } else {
    cout << "No path found!" << endl;
  }

  return 0;
}
