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

// Function to find GCD (Greatest Common Divisor)
int gcd(int a, int b) {
  a = abs(a);
  b = abs(b);
  while (b != 0) {
    int temp = b;
    b = a % b;
    a = temp;
  }
  return a;
}

int main() {
  ifstream file("input.txt");
  if (!file.is_open()) {
    cerr << "Error: Could not open input.txt" << endl;
    return 1;
  }

  vector<string> grid;
  string line;

  // Read the grid
  while (getline(file, line)) {
    grid.push_back(line);
  }
  file.close();

  if (grid.empty()) {
    cerr << "Error: Empty grid" << endl;
    return 1;
  }

  int rows = grid.size();
  int cols = grid[0].size();

  // Find all antennas grouped by frequency
  map<char, vector<Position>> antennas;

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      char ch = grid[r][c];
      if (ch != '.') {
        antennas[ch].push_back(Position(r, c));
      }
    }
  }

  // Set to store unique antinode positions
  set<Position> antinodes;

  // For each frequency, find antinodes for all pairs of antennas
  for (auto &[freq, positions] : antennas) {
    cout << "Processing frequency '" << freq << "' with " << positions.size()
         << " antennas" << endl;

    // If there are at least 2 antennas of this frequency,
    // all antenna positions are also antinodes
    if (positions.size() >= 2) {
      for (const Position &pos : positions) {
        antinodes.insert(pos);
      }
    }

    // Check all pairs of antennas with the same frequency
    for (size_t i = 0; i < positions.size(); i++) {
      for (size_t j = i + 1; j < positions.size(); j++) {
        Position ant1 = positions[i];
        Position ant2 = positions[j];

        // Calculate direction vector and reduce it to smallest integer form
        int dr = ant2.row - ant1.row;
        int dc = ant2.col - ant1.col;

        int g = gcd(dr, dc);
        if (g > 0) {
          dr /= g;
          dc /= g;
        }

        // Find all positions along the line in both directions
        // Starting from ant1, go in both directions

        // Direction towards ant2 and beyond
        int r = ant1.row;
        int c = ant1.col;
        while (r >= 0 && r < rows && c >= 0 && c < cols) {
          antinodes.insert(Position(r, c));
          r += dr;
          c += dc;
        }

        // Direction opposite to ant2
        r = ant1.row - dr;
        c = ant1.col - dc;
        while (r >= 0 && r < rows && c >= 0 && c < cols) {
          antinodes.insert(Position(r, c));
          r -= dr;
          c -= dc;
        }
      }
    }
  }

  // Print the grid with antinodes marked
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
