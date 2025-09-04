#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

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
  int count = 0;

  // Check each possible center position for an X-MAS pattern
  for (int i = 1; i < rows - 1; ++i) {
    for (int j = 1; j < cols - 1; ++j) {
      // Center must be 'A'
      if (grid[i][j] != 'A')
        continue;

      // Check the two diagonals
      // Diagonal 1: top-left to bottom-right
      char tl = grid[i - 1][j - 1]; // Top-left
      char br = grid[i + 1][j + 1]; // Bottom-right

      // Diagonal 2: top-right to bottom-left
      char tr = grid[i - 1][j + 1]; // Top-right
      char bl = grid[i + 1][j - 1]; // Bottom-left

      // For each diagonal, we need either 'MAS' or 'SAM'
      // Diagonal 1: either (M,A,S) or (S,A,M)
      bool diag1_valid = (tl == 'M' && br == 'S') || (tl == 'S' && br == 'M');

      // Diagonal 2: either (M,A,S) or (S,A,M)
      bool diag2_valid = (tr == 'M' && bl == 'S') || (tr == 'S' && bl == 'M');

      // Both diagonals must form valid MAS patterns
      if (diag1_valid && diag2_valid) {
        count++;
      }
    }
  }

  cout << "X-MAS appears " << count << " times." << endl;

  return 0;
}
