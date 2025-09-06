#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Position {
  int row, col;

  Position(int r, int c) : row(r), col(c) {}

  bool operator<(const Position &other) const {
    if (row != other.row)
      return row < other.row;
    return col < other.col;
  }
};

// Directions: up, right, down, left
int dr[] = {-1, 0, 1, 0};
int dc[] = {0, 1, 0, -1};

int calculateTrailheadRating(const vector<string> &map, int startRow,
                             int startCol) {
  int rows = map.size();
  int cols = map[0].size();

  // Use DFS to count all distinct paths to height 9
  function<int(int, int, int)> dfs = [&](int row, int col,
                                         int expectedHeight) -> int {
    // Check bounds
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
      return 0;
    }

    int currentHeight = map[row][col] - '0';

    // Check if this position has the expected height
    if (currentHeight != expectedHeight) {
      return 0;
    }

    // If we reached height 9, this is one complete trail
    if (currentHeight == 9) {
      return 1;
    }

    // Count trails from all 4 directions
    int totalTrails = 0;
    for (int dir = 0; dir < 4; dir++) {
      int newRow = row + dr[dir];
      int newCol = col + dc[dir];
      totalTrails += dfs(newRow, newCol, currentHeight + 1);
    }

    return totalTrails;
  };

  return dfs(startRow, startCol, 0);
}

int main() {
  ifstream file("input.txt");
  if (!file.is_open()) {
    cerr << "Error: Could not open input.txt" << endl;
    return 1;
  }

  vector<string> map;
  string line;

  while (getline(file, line)) {
    map.push_back(line);
  }
  file.close();

  if (map.empty()) {
    cerr << "Error: Empty map" << endl;
    return 1;
  }

  int rows = map.size();
  int cols = map[0].size();

  cout << "Topographic map (" << rows << "x" << cols << "):" << endl;
  for (const string &row : map) {
    cout << row << endl;
  }
  cout << endl;

  // Find all trailheads (positions with height 0)
  vector<Position> trailheads;
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      if (map[r][c] == '0') {
        trailheads.push_back(Position(r, c));
      }
    }
  }

  cout << "Found " << trailheads.size() << " trailheads:" << endl;

  int totalRating = 0;
  for (size_t i = 0; i < trailheads.size(); i++) {
    Position trailhead = trailheads[i];
    int rating = calculateTrailheadRating(map, trailhead.row, trailhead.col);

    cout << "Trailhead " << (i + 1) << " at (" << trailhead.row << ","
         << trailhead.col << ") has rating: " << rating << endl;

    totalRating += rating;
  }

  cout << "\nSum of all trailhead ratings: " << totalRating << endl;

  return 0;
}
