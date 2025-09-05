#include <fstream>
#include <iostream>
#include <map>
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

int main() {
  ifstream file("input.txt");

  vector<string> grid;
  string line;

  while (getline(file, line)) {
    grid.push_back(line);
  }
  file.close();

  int rows = grid.size();
  int cols = grid[0].size();

  map<char, vector<Position>> antennas;

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      char ch = grid[r][c];
      if (ch != '.') {
        antennas[ch].push_back(Position(r, c));
      }
    }
  }

  set<Position> antinodes;

  for (auto &[freq, positions] : antennas) {
    cout << "Processing frequency '" << freq << "' with " << positions.size()
         << " antennas" << endl;

    // Check all pairs of antennas with the same frequency
    for (size_t i = 0; i < positions.size(); i++) {
      for (size_t j = i + 1; j < positions.size(); j++) {
        Position ant1 = positions[i];
        Position ant2 = positions[j];

        // Calculate the two possible antinode positions
        // Antinode 1: where ant2 is twice as far as ant1
        int antinode1_r = 2 * ant2.row - ant1.row;
        int antinode1_c = 2 * ant2.col - ant1.col;

        // Antinode 2: where ant1 is twice as far as ant2
        int antinode2_r = 2 * ant1.row - ant2.row;
        int antinode2_c = 2 * ant1.col - ant2.col;

        // Check if antinodes are within bounds and add them
        if (antinode1_r >= 0 && antinode1_r < rows && antinode1_c >= 0 &&
            antinode1_c < cols) {
          antinodes.insert(Position(antinode1_r, antinode1_c));
          cout << "  Antinode at (" << antinode1_r << "," << antinode1_c << ")"
               << endl;
        }

        if (antinode2_r >= 0 && antinode2_r < rows && antinode2_c >= 0 &&
            antinode2_c < cols) {
          antinodes.insert(Position(antinode2_r, antinode2_c));
          cout << "  Antinode at (" << antinode2_r << "," << antinode2_c << ")"
               << endl;
        }
      }
    }
  }

  cout << "\nGrid with antinodes marked as '#':" << endl;
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      if (antinodes.count(Position(r, c))) {
        if (grid[r][c] == '.') {
          cout << '#';
        } else {
          cout << grid[r][c]; // Show original antenna if antinode overlaps
        }
      } else {
        cout << grid[r][c];
      }
    }
    cout << endl;
  }

  cout << "\nTotal unique antinode locations: " << antinodes.size() << endl;

  return 0;
}
