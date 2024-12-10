#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <unordered_map>

using namespace std;

class EngineSchematicAnalyzer {
	private:
		vector<string> schematic;
		unordered_map<string, vector<int>> gearParts;

		string coordToKey(int row, int col) {
			return to_string(row) + "," + to_string(col);
		}

		void checkAdjacentGears(int row, int startCol, int endCol, int number) {
			int minRow = max(0, row - 1);
			int maxRow = min(static_cast<int>(schematic.size()) - 1, row + 1);
			int minCol = max(0, startCol - 1);
			int maxCol = min(static_cast<int>(schematic[0].length()) - 1, endCol + 1);

			for (int r = minRow; r <= maxRow; ++r) {
				for (int c = minCol; c <= maxCol; ++c) {
					if (r == row && c >= startCol && c <= endCol) continue;

					if (schematic[r][c] == '*') {
						string gearKey = coordToKey(r, c);
						gearParts[gearKey].push_back(number);
					}
				}
			}
		}

	public:
		bool loadSchematic(const string& filename) {
			ifstream file(filename);
			if (!file.is_open()) {
				cerr << "Error: unable to open file " << filename << endl;
				return false;
			}

			schematic.clear();
			gearParts.clear();
			string line;
			while(getline(file, line)) {
				schematic.push_back(line);
			}
			return true;
		}

		int calculateGearRatioSum() {
			for(int row = 0; row < schematic.size(); ++row) {
				for(int col = 0; col < schematic[row].length(); ++col) {
					if (isdigit(schematic[row][col])) {
						int startCol = col;
						int number = 0;
						while(col < schematic[row].length() && isdigit(schematic[row][col])) {
							number = number * 10 + (schematic[row][col] - '0');
							col++;
						}

						checkAdjacentGears(row, startCol, col - 1, number);

						col--;
					}
				}
			}

			int totalGearRatioSum = 0;
			for (const auto& [gear, parts] : gearParts) {
				if (parts.size() == 2) {
					totalGearRatioSum += parts[0] * parts[1];
				}
			}

			return totalGearRatioSum;

		}
};

int main() {
	EngineSchematicAnalyzer analyzer;

	if (analyzer.loadSchematic("input.txt")) {
		int result = analyzer.calculateGearRatioSum();
		cout << "Sum of gear ratios is: " << result << endl;
	}

	return 0;
}
