#include <fstream>
#include <iostream>
#include <queue>
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

struct Region {
  char plant;
  int area;
  int perimeter;
  vector<Position> plots;
};

int dr[] = {-1, 0, 1, 0};
int dc[] = {0, 1, 0, -1};

bool isValid(int row, int col, int rows, int cols) {
  return row >= 0 && row < rows && col >= 0 && col < cols;
}

Region findRegion(const vector<string> &garden, vector<vector<bool>> &visited,
                  int startRow, int startCol) {
  int rows = garden.size();
  int cols = garden[0].size();
  char plant = garden[startRow][startCol];

  Region region;
  region.plant = plant;
  region.area = 0;
  region.perimeter = 0;

  queue<Position> q;
  q.push(Position(startRow, startCol));
  visited[startRow][startCol] = true;

  while (!q.empty()) {
    Position current = q.front();
    q.pop();

    region.plots.push_back(current);
    region.area++;

    int plotPerimeter = 0;

    for (int dir = 0; dir < 4; dir++) {
      int newRow = current.row + dr[dir];
      int newCol = current.col + dc[dir];

      if (!isValid(newRow, newCol, rows, cols) ||
          garden[newRow][newCol] != plant) {
        plotPerimeter++;
      } else if (!visited[newRow][newCol]) {
        visited[newRow][newCol] = true;
        q.push(Position(newRow, newCol));
      }
    }

    region.perimeter += plotPerimeter;
  }
  return region;
}

int main() {
  ifstream file("input.txt");
  vector<string> garden;
  string line;
  while (getline(file, line)) {
    garden.push_back(line);
  }
  file.close();

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
    long long price = (long long)region.area * region.perimeter;
    totalPrice += price;

    cout << "Region " << (i + 1) << ": Plant " << region.plant << ", Area "
         << region.area << ", Perimeter " << region.perimeter << ", Price "
         << price << endl;
  }

  cout << "\nTotal price of fencing all regions: " << totalPrice << endl;

  return 0;
}
