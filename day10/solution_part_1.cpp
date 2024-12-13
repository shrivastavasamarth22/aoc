#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <utility>
#include <climits>

using namespace std;

class PipeLoopSolver {
	private:
		vector<string> grid;
		int startRow, startCol;
		int rows, cols;

		const vector<pair<int, int>> getConnections(char pipe) {
			switch (pipe) {
				case '|': return {{-1, 0}, {1, 0}};     // North and South
				case '-': return {{0, -1}, {0, 1}};     // West and East
				case 'L': return {{-1, 0}, {0, 1}};     // North and East
				case 'J': return {{-1, 0}, {0, -1}};    // North and West
				case '7': return {{1, 0}, {0, -1}};     // South and West
				case 'F': return {{1, 0}, {0, 1}};      // South and East
				default: return {};
			}
		}

		char findStartPipeType() {
			for (char pipe : {'|', '-', 'L', 'J', '7', 'F'}) {
				if (isValidStartPipe(pipe)) return pipe;
			}
			return '.';
		}

		bool isValidStartPipe(char pipe) {
			vector<pair<int, int>> connections = getConnections(pipe);
			int validConnections = 0;

			for (auto [dr, dc] : connections) {
				int newRow = startRow + dr;
				int newCol = startCol + dc;

				if (newRow >= 0 && newRow < rows && 
						newCol >= 0 && newCol < cols) {
					char neighborPipe = grid[newRow][newCol];

					for (auto [oppDr, oppDc] : getConnections(neighborPipe)) {
						if (newRow + oppDr == startRow && 
								newCol + oppDc == startCol) {
							validConnections++;
							break;
						}
					}
				}
			}

			return validConnections == 2;
		}
	
	public:
		PipeLoopSolver(const string& filename) {
			ifstream file(filename);
			string line;

			while(getline(file, line)) {
				grid.push_back(line);
			}

			rows = grid.size();
			cols = grid[0].length();

			for (int r = 0; r < rows; r++) {
				for (int c = 0; c < cols; c++) {
					if (grid[r][c] == 'S') {
						startRow = r;
						startCol = c;
						break;
					}
				}
			}
		}

		int findFarthestDistace() {
			char startPipeType = findStartPipeType();
			grid[startRow][startCol] = startPipeType;

			vector<vector<int>> distances(rows, vector<int>(cols, INT_MAX));
			distances[startRow][startCol] = 0;

			queue<pair<int, int>> queue;
			queue.push({startRow, startCol});

			int maxDistance = 0;

			while (!queue.empty()) {
				auto [row, col] = queue.front();
				queue.pop();

				for (auto [dr, dc] : getConnections(grid[row][col])) {
					int newRow = row + dr;
					int newCol = col + dc;

					if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
						bool validConnection = false;
						for (auto [oppDr, oppDc] : getConnections(grid[newRow][newCol])) {
							if (newRow + oppDr == row && newCol + oppDc == col) {
								validConnection = true;
								break;
							}
						}

						if (validConnection && distances[newRow][newCol] == INT_MAX) {
							distances[newRow][newCol] = distances[row][col] + 1;
							maxDistance = max(maxDistance, distances[newRow][newCol]);
							queue.push({newRow, newCol});
						}
					}
				}
			}

			return maxDistance;
		}
};

int main() {
	PipeLoopSolver solver("input.txt");
	cout << solver.findFarthestDistace() << endl;
	return 0;
}
