#include <fstream>
#include <iostream>
#include <queue>
#include <set>
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

int dr[] = {-1, 0, 1, 0};
int dc[] = {0, 1, 0, -1};

int calculateTrailheadScore(const vector<string> &map, int startRow,
                            int startCol) {
  int rows = map.size();
  int cols = map[0].size();

  set<Position> reachableNines;
  queue<Position> q;
  set<Position> visited;

  q.push(Position(startRow, startCol));
  visited.insert(Position(startRow, startCol));

  while (!q.empty()) {
    Position current = q.front();
    q.pop();

    int currentHeight = map[current.row][current.col] - '0';

    if (currentHeight == 9) {
      reachableNines.insert(current);
      continue;
    }

    for (int dir = 0; dir < 4; dir++) {
      int newRow = current.row + dr[dir];
      int newCol = current.col + dc[dir];

      // Check bounds
      if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= cols) {
        continue;
      }

      Position newPos(newRow, newCol);

      // Skip if already visited
      if (visited.count(newPos)) {
        continue;
      }

      int newHeight = map[newRow][newCol] - '0';

      // Check if this is a valid hiking step (increases by exactly 1)
      if (newHeight == currentHeight + 1) {
        visited.insert(newPos);
        q.push(newPos);
      }
    }
  }

  return reachableNines.size();
}

int main() {
  ifstream file("input.txt");
  vector<string> map;
  string line;

  while (getline(file, line)) {
    map.push_back(line);
  }
  file.close();

  int rows = map.size();
  int cols = map[0].size();

  for (const string &row : map) {
    cout << row << endl;
  }
  cout << endl;

  vector<Position> trailheads;
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      if (map[r][c] == '0') {
        trailheads.push_back(Position(r, c));
      }
    }
  }

  cout << "Found " << trailheads.size() << " trailheads." << endl;

  int totalScore = 0;
  for (size_t i = 0; i < trailheads.size(); i++) {
    Position trailhead = trailheads[i];
    int score = calculateTrailheadScore(map, trailhead.row, trailhead.col);

    cout << "Trailhead " << (i + 1) << " at (" << trailhead.row << ","
         << trailhead.col << ") has score: " << score << endl;
    totalScore += score;
  }
  cout << "\nTotal score of all trailheads: " << totalScore << endl;
  return 0;
}
