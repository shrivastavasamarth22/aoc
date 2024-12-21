#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool checkMAS(const vector<string>& grid, int row, int col, int drow, int dcol) {
	// Check if "MAS" exists starting from (row,col) in direction (drow,dcol)
	// Also check for "SAM" (backwards case)
	if (row < 0 || row >= grid.size() || col < 0 || col >= grid[0].size()) return false;
	if (row + 2*drow < 0 || row + 2*drow >= grid.size()) return false;
	if (col + 2*dcol < 0 || col + 2*dcol >= grid[0].size()) return false;

	// Check forward direction (MAS)
	bool forward = grid[row][col] == 'M' &&
		grid[row + drow][col + dcol] == 'A' &&
		grid[row + 2*drow][col + 2*dcol] == 'S';

	// Check backward direction (SAM)
	bool backward = grid[row][col] == 'S' &&
		grid[row + drow][col + dcol] == 'A' &&
		grid[row + 2*drow][col + 2*dcol] == 'M';

	return forward || backward;
}

int countXMAS(const vector<string>& grid) {
	int count = 0;
	int rows = grid.size();
	int cols = grid[0].size();

	// For each cell as the center of potential X
	for (int i = 1; i < rows - 1; i++) {
		for (int j = 1; j < cols - 1; j++) {
			// Check if this is an 'A' (center of X)
			if (grid[i][j] != 'A') continue;

			// Check all possible X patterns centered at this 'A'
			// We need to check both diagonals, and each diagonal can be MAS or SAM in either direction

			// Check top-left to bottom-right and bottom-left to top-right
			bool topLeft = checkMAS(grid, i-1, j-1, 1, 1);
			bool bottomRight = checkMAS(grid, i+1, j+1, -1, -1);
			bool topRight = checkMAS(grid, i-1, j+1, 1, -1);
			bool bottomLeft = checkMAS(grid, i+1, j-1, -1, 1);

			// If we have MAS/SAM in both diagonals
			if ((topLeft && bottomRight) || (topRight && bottomLeft)) {
				count++;
			}
		}
	}

	return count;
}

int main() {

	ifstream file("input.txt");
	if (!file.is_open()) {
		cout << "File not found!" << endl;
		return 1;
	}


	vector<string> grid;
	string line;

	// Read input
	while (getline(cin, line)) {
		if (line.empty()) break;
		grid.push_back(line);
	}

	// Count X-MAS patterns
	int result = countXMAS(grid);
	cout << result << endl;

	return 0;
}
