#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
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

  pair<int, int> solve() {
    // First, find minimum costs to reach each state
    priority_queue<State, vector<State>, greater<State>> pq;
    map<StateKey, int> dist;
    map<StateKey, vector<StateKey>> predecessors;

    // Start facing East (direction 0)
    StateKey startKey(start, 0);
    pq.push(State(start, 0, 0));
    dist[startKey] = 0;

    int minEndCost = INT_MAX;

    while (!pq.empty()) {
      State current = pq.top();
      pq.pop();

      StateKey currentKey(current.pos, current.dir);

      // If we've found a better path to this state, skip
      if (dist.count(currentKey) && dist[currentKey] < current.cost) {
        continue;
      }

      // If we reached the end, update minimum cost
      if (current.pos == end) {
        minEndCost = min(minEndCost, current.cost);
      }

      // If current cost is already worse than best end cost, skip
      if (current.cost > minEndCost) {
        continue;
      }

      // Try moving forward
      Position nextPos = current.pos + directions[current.dir];
      if (isValid(nextPos)) {
        StateKey nextKey(nextPos, current.dir);
        int newCost = current.cost + 1;

        if (!dist.count(nextKey) || dist[nextKey] > newCost) {
          dist[nextKey] = newCost;
          predecessors[nextKey].clear();
          predecessors[nextKey].push_back(currentKey);
          pq.push(State(nextPos, current.dir, newCost));
        } else if (dist[nextKey] == newCost) {
          predecessors[nextKey].push_back(currentKey);
        }
      }

      // Try turning clockwise
      int clockwiseDir = (current.dir + 1) % 4;
      StateKey clockwiseKey(current.pos, clockwiseDir);
      int newCost = current.cost + 1000;

      if (!dist.count(clockwiseKey) || dist[clockwiseKey] > newCost) {
        dist[clockwiseKey] = newCost;
        predecessors[clockwiseKey].clear();
        predecessors[clockwiseKey].push_back(currentKey);
        pq.push(State(current.pos, clockwiseDir, newCost));
      } else if (dist[clockwiseKey] == newCost) {
        predecessors[clockwiseKey].push_back(currentKey);
      }

      // Try turning counterclockwise
      int counterclockwiseDir = (current.dir + 3) % 4;
      StateKey counterclockwiseKey(current.pos, counterclockwiseDir);

      if (!dist.count(counterclockwiseKey) ||
          dist[counterclockwiseKey] > newCost) {
        dist[counterclockwiseKey] = newCost;
        predecessors[counterclockwiseKey].clear();
        predecessors[counterclockwiseKey].push_back(currentKey);
        pq.push(State(current.pos, counterclockwiseDir, newCost));
      } else if (dist[counterclockwiseKey] == newCost) {
        predecessors[counterclockwiseKey].push_back(currentKey);
      }
    }

    // Now backtrack from all end states with minimum cost to find all optimal
    // paths
    set<Position> optimalTiles;
    queue<StateKey> backtrack;
    set<StateKey> visited;

    // Add all end states with minimum cost
    for (int dir = 0; dir < 4; dir++) {
      StateKey endKey(end, dir);
      if (dist.count(endKey) && dist[endKey] == minEndCost) {
        backtrack.push(endKey);
        visited.insert(endKey);
      }
    }

    while (!backtrack.empty()) {
      StateKey current = backtrack.front();
      backtrack.pop();

      optimalTiles.insert(current.pos);

      if (predecessors.count(current)) {
        for (const StateKey &pred : predecessors[current]) {
          if (visited.find(pred) == visited.end()) {
            visited.insert(pred);
            backtrack.push(pred);
          }
        }
      }
    }

    return {minEndCost, optimalTiles.size()};
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

  auto [minScore, tilesOnOptimalPaths] = maze.solve();

  if (minScore != INT_MAX) {
    cout << "Part 1 - Minimum score: " << minScore << endl;
    cout << "Part 2 - Tiles on optimal paths: " << tilesOnOptimalPaths << endl;
  } else {
    cout << "No path found!" << endl;
  }

  return 0;
}
