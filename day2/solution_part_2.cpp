#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

class CubeGameAnalyzer {
private:
    // Find the minimum number of cubes needed for a game
    unordered_map<string, int> findMinimumCubes(const string& gameData) {
        // Initialize minimum cubes to 0
        unordered_map<string, int> minCubes = {
            {"red", 0},
            {"green", 0},
            {"blue", 0}
        };
        
        // Find the first colon to skip the game ID part
        size_t colonPos = gameData.find(':');
        if (colonPos == string::npos) return minCubes;
        
        string subsets = gameData.substr(colonPos + 1);
        istringstream iss(subsets);
        string subset;
        
        // Check each subset of cubes
        while (getline(iss, subset, ';')) {
            istringstream subsetStream(subset);
            string colorCount;
            
            // Check each color count in the subset
            while (getline(subsetStream, colorCount, ',')) {
                // Trim leading whitespace
                colorCount.erase(0, colorCount.find_first_not_of(" "));
                
                // Parse color count
                istringstream countStream(colorCount);
                int count;
                string color;
                countStream >> count >> color;
                
                // Update minimum cubes needed
                minCubes[color] = max(minCubes[color], count);
            }
        }
        
        return minCubes;
    }

public:
    // Calculate the sum of cube set powers
    int sumCubeSetPowers(const string& filename) {
        ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return -1;
        }
        
        string line;
        int totalPower = 0;
        
        while (getline(inputFile, line)) {
            // Find minimum cubes needed
            auto minCubes = findMinimumCubes(line);
            
            // Calculate power (multiplication of min cubes)
            int power = minCubes["red"] * minCubes["green"] * minCubes["blue"];
            
            // Add to total power
            totalPower += power;
        }
        
        return totalPower;
    }
};

int main() {
	CubeGameAnalyzer analyzer;

	int result = analyzer.sumCubeSetPowers("input.txt");

	if (result != -1) {
		cout << "Sum of cube set powers: " << result << endl;
	}

	return 0;
}
