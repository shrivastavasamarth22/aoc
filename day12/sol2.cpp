#include <fstream>
#include <iostream>
#include <queue>
<<<<<<< HEAD
#include <set>
=======
>>>>>>> 91421c5a7b6ddc40fcfa1b4e0009862e2e2f86fd
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

  bool operator==(const Position &other) const {
    return row == other.row && col == other.col;
  }
};

<<<<<<< HEAD
struct FenceSegment {
  int row, col;
  int direction;

  FenceSegment(int r, int c, int d) : row(r), col(c), direction(d) {}

  bool operator<(const FenceSegment &other) const {
    if (row != other.row)
      return row < other.row;
    if (col != other.col)
      return col < other.col;
    return direction < other.direction;
  }
};

struct Region {
  char plant;
  int area;
  int sides;
  vector<Position> plots;
  set<FenceSegment> fenceSegments;
=======
struct Region {
  char plant;
  int area;
  int perimeter;
  vector<Position> plots;
>>>>>>> 91421c5a7b6ddc40fcfa1b4e0009862e2e2f86fd
};

int dr[] = {-1, 0, 1, 0};
int dc[] = {0, 1, 0, -1};

bool isValid(int row, int col, int rows, int cols) {
  return row >= 0 && row < rows && col >= 0 && col < cols;
}

<<<<<<< HEAD
int countSides(const set<FenceSegment> &fenceSegments) {
  set<FenceSegment> visited;
  int sideCount = 0;

  for (const FenceSegment &segment : fenceSegments) {
    if (visited.count(segment))
      continue;

    sideCount++;
    queue<FenceSegment> q;
    q.push(segment);
    visited.insert(segment);

    while (!q.empty()) {
      FenceSegment current = q.front();
      q.pop();

      if (current.direction == 0 || current.direction == 2) {
        for (int dc = -1; dc <= 1; dc += 2) {
          FenceSegment adjacent(current.row, current.col + dc,
                                current.direction);
          if (fenceSegments.count(adjacent) && !visited.count(adjacent)) {
            visited.insert(adjacent);
            q.push(adjacent);
          }
        }
      } else {
        for (int dr = -1; dr <= 1; dr += 2) {
          FenceSegment adjacent(current.row + dr, current.col,
                                current.direction);
          if (fenceSegments.count(adjacent) && !visited.count(adjacent)) {
            visited.insert(adjacent);
            q.push(adjacent);
          }
        }
      }
    }
  }

  return sideCount;
}

=======
>>>>>>> 91421c5a7b6ddc40fcfa1b4e0009862e2e2f86fd
Region findRegion(const vector<string> &garden, vector<vector<bool>> &visited,
                  int startRow, int startCol) {
  int rows = garden.size();
  int cols = garden[0].size();
  char plant = garden[startRow][startCol];

  Region region;
  region.plant = plant;
  region.area = 0;
<<<<<<< HEAD
=======
  region.perimeter = 0;
>>>>>>> 91421c5a7b6ddc40fcfa1b4e0009862e2e2f86fd

  queue<Position> q;
  q.push(Position(startRow, startCol));
  visited[startRow][startCol] = true;

  while (!q.empty()) {
    Position current = q.front();
    q.pop();

    region.plots.push_back(current);
    region.area++;

<<<<<<< HEAD
=======
    int plotPerimeter = 0;

>>>>>>> 91421c5a7b6ddc40fcfa1b4e0009862e2e2f86fd
    for (int dir = 0; dir < 4; dir++) {
      int newRow = current.row + dr[dir];
      int newCol = current.col + dc[dir];

      if (!isValid(newRow, newCol, rows, cols) ||
          garden[newRow][newCol] != plant) {
<<<<<<< HEAD
        region.fenceSegments.insert(
            FenceSegment(current.row, current.col, dir));
=======
        plotPerimeter++;
>>>>>>> 91421c5a7b6ddc40fcfa1b4e0009862e2e2f86fd
      } else if (!visited[newRow][newCol]) {
        visited[newRow][newCol] = true;
        q.push(Position(newRow, newCol));
      }
    }
<<<<<<< HEAD
  }

  region.sides = countSides(region.fenceSegments);

=======

    region.perimeter += plotPerimeter;
  }
>>>>>>> 91421c5a7b6ddc40fcfa1b4e0009862e2e2f86fd
  return region;
}

int main() {
  ifstream file("input.txt");
<<<<<<< HEAD
  if (!file.is_open()) {
    cerr << "Error: Could not open input.txt" << endl;
    return 1;
  }

  vector<string> garden;
  string line;

=======
  vector<string> garden;
  string line;
>>>>>>> 91421c5a7b6ddc40fcfa1b4e0009862e2e2f86fd
  while (getline(file, line)) {
    garden.push_back(line);
  }
  file.close();

<<<<<<< HEAD
  if (garden.empty()) {
    cerr << "Error: Empty garden map" << endl;
    return 1;
  }

=======
>>>>>>> 91421c5a7b6ddc40fcfa1b4e0009862e2e2f86fd
  int rows = garden.size();
  int cols = garden[0].size();

  cout << "Garden map (" << rows << "x" << cols << "):" << endl;
  for (const string &row : garden) {
    cout << row << endl;
  }
  cout << endl;

  vector<vector<bool>> visited(rows, vector<bool>(cols, false));
  vector<Region> regions;

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      if (!visited[r][c]) {
        Region region = findRegion(garden, visited, r, c);
        regions.push_back(region);
      }
    }
  }

  cout << "Found " << regions.size() << " regions:" << endl;

  long long totalPrice = 0;

  for (size_t i = 0; i < regions.size(); i++) {
    Region &region = regions[i];
<<<<<<< HEAD
    long long price = (long long)region.area * region.sides;
    totalPrice += price;

    cout << "Region " << (i + 1) << ": Plant " << region.plant << ", Area "
         << region.area << ", Sides " << region.sides << ", Price " << price
         << endl;
=======
    long long price = (long long)region.area * region.perimeter;
    totalPrice += price;

    cout << "Region " << (i + 1) << ": Plant " << region.plant << ", Area "
         << region.area << ", Perimeter " << region.perimeter << ", Price "
         << price << endl;
>>>>>>> 91421c5a7b6ddc40fcfa1b4e0009862e2e2f86fd
  }

  cout << "\nTotal price of fencing all regions: " << totalPrice << endl;

  return 0;
}
