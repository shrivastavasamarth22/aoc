#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

class GalaxyMapper {
	private:
		vector<string> universe;
		vector<int> emptyRows;
		vector<int> emptyCols;
		vector<pair<int, int>> galaxyPositions;

		void findEmptySpaces() {
			for (int r = 0; r < universe.size(); ++r) {
				if (all_of(universe[r].begin(), universe[r].end(), [](char c) { return c == '.'; })) {
					emptyRows.push_back(r);
				}
			}

			for (int c = 0; c < universe[0].size(); ++c) {
				bool isEmpty = true;
				for (int r = 0; r < universe.size(); ++r) {
					if (universe[r][c] == '#') {
						isEmpty = false;
						break;
					}
				}
				if (isEmpty) {
					emptyCols.push_back(c);
				}
			}
		}

		void mapGalaxies() {
			for (int r = 0; r < universe.size(); ++r) {
				for (int c = 0; c < universe[r].size(); ++c) {
					if (universe[r][c] == '#') {
						galaxyPositions.push_back({r, c});
					}
				}
			}
		}

		long long calculateDistance(const pair<int, int>& galaxy1, const pair<int, int>& galaxy2) {
			long long distance = abs(galaxy1.first - galaxy2.first) + abs(galaxy1.second - galaxy2.second);

			for (int row : emptyRows) {
				if (row > min(galaxy1.first, galaxy2.first) &&
					row < max(galaxy1.first, galaxy2.first)) {
					distance++;
				}
			}

			for (int col : emptyCols) {
				if (col > min(galaxy1.second, galaxy2.second) &&
						col < max(galaxy1.second, galaxy2.second)) {
					distance++;
				}
			}

			return distance;
		}
	
	public:
		GalaxyMapper(const vector<string>& input) : universe(input) {
			findEmptySpaces();
			mapGalaxies();
		}

		long long sumShortestPaths() {
			long long totalDistance = 0;

			for (size_t i = 0; i < galaxyPositions.size(); ++i) {
				for (size_t j = i + 1; j < galaxyPositions.size(); ++j) {
					totalDistance += calculateDistance(galaxyPositions[i], galaxyPositions[j]);
				}
			}

			return totalDistance;
		}
};

int main() {
	ifstream file("input.txt");
	if (!file) {
		cerr << "Error opening input.txt" << endl;
		return 1;
	}

	vector<string> input;
	string line;
	while(getline(file, line)) {
		input.push_back(line);
	}
	file.close();

	GalaxyMapper mapper(input);
	cout << "Sum of shortest paths: " << mapper.sumShortestPaths() << endl;

	return 0;
}
