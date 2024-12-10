#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

class EngineSchematicAnalyzer {
	private:
		vector<string> schematic;

		bool isSymbol(char c) {
			return c != '.' && !isdigit(c);
		}

		bool isPartNumber(int row, int startCol, int endCol) {
			int minRow = max(0, row - 1);
			int maxRow = min(static_cast<int>(schematic.size()) - 1, row + 1);
			int minCol = max(0, startCol - 1);
			int maxCol = min(static_cast<int>(schematic[0].length()) - 1, endCol + 1);

			for(int r = minRow; r <= maxRow; ++r) {
				for(int c = minCol; c <= maxCol; ++c) {
					if (r == row && c >= startCol && c <= endCol) continue;

					if (isSymbol(schematic[r][c])) {
						return true;
					}
				}
			}
			return false;
		}

	public:
		bool loadSchematic(const string& filename) {
			ifstream file(filename);
			if (!file.is_open()) {
				cerr << "Error: unable to open file " << filename << endl;
				return false;
			}

			schematic.clear();
			string line;
			while(getline(file, line)) {
				schematic.push_back(line);
			}
			return true;
		}

		int calculatePartNumberSum() {
			int sum = 0;
			for(int row = 0; row < schematic.size(); ++row) {
				for(int col = 0; col < schematic[row].length(); ++col) {
					if (isdigit(schematic[row][col])) {
						int startCol = col;
						int number = 0;
						while (col < schematic[row].length() && isdigit(schematic[row][col])) {
							number = number * 10 + (schematic[row][col] - '0');
							col++;
						}
						if (isPartNumber(row, startCol, col - 1)) {
							sum += number; 
						}

						col--;
					}
				}
			}
			return sum;
		}
};

int main() {
	EngineSchematicAnalyzer analyzer;

	if (analyzer.loadSchematic("input.txt")) {
		int result = analyzer.calculatePartNumberSum();
		cout << "The sum of part numbers is: " << result << endl;
	}

	return 0;
}
