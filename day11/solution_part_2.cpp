#include <iostream>
#include <fstream>
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
    long long expansionFactor;

    void findEmptySpaces() {
        // Find empty rows
        for (int r = 0; r < universe.size(); ++r) {
            if (all_of(universe[r].begin(), universe[r].end(), [](char c) { return c == '.'; })) {
                emptyRows.push_back(r);
            }
        }

        // Find empty columns
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

        // Add expanded distance for empty rows and columns
        for (int row : emptyRows) {
            if (row > min(galaxy1.first, galaxy2.first) && 
                row < max(galaxy1.first, galaxy2.first)) {
                distance += expansionFactor - 1;
            }
        }

        for (int col : emptyCols) {
            if (col > min(galaxy1.second, galaxy2.second) && 
                col < max(galaxy1.second, galaxy2.second)) {
                distance += expansionFactor - 1;
            }
        }

        return distance;
    }

public:
    GalaxyMapper(const vector<string>& input, long long expansion = 2) 
        : universe(input), expansionFactor(expansion) {
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
    // Read input from file
    ifstream inputFile("input.txt");
    if (!inputFile) {
        cerr << "Error opening input.txt" << endl;
        return 1;
    }

    vector<string> input;
    string line;
    while (getline(inputFile, line)) {
        input.push_back(line);
    }
    inputFile.close();

    GalaxyMapper mapper(input, 1000000);
    cout << "Sum of shortest paths (Part 2): " << mapper.sumShortestPaths() << endl;

    return 0;
}
