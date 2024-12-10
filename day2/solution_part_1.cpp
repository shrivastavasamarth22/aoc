#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

class CubeGameValidator {
	private:
		// Maximum cubes allowed for each color
		const std::unordered_map<std::string, int> MAX_CUBES = {
			{"red", 12},
			{"green", 13},
			{"blue", 14}
		};

		// Parse a single color-count pair (e.g., "3 blue")
		bool parseColorCount(const std::string& colorCount) {
			std::istringstream iss(colorCount);
			int count;
			std::string color;

			iss >> count >> color;

			// Check if this color count exceeds the maximum allowed
			return count <= MAX_CUBES.at(color);
		}

	public:
		// Check if a single game is possible
		bool isGamePossible(const std::string& gameData) {
			// Find the first colon to skip the game ID part
			size_t colonPos = gameData.find(':');
			if (colonPos == std::string::npos) return false;

			std::string subsets = gameData.substr(colonPos + 1);
			std::istringstream iss(subsets);
			std::string subset;

			// Check each subset of cubes
			while (std::getline(iss, subset, ';')) {
				std::istringstream subsetStream(subset);
				std::string colorCount;

				// Check each color count in the subset
				while (std::getline(subsetStream, colorCount, ',')) {
					// Trim leading whitespace
					colorCount.erase(0, colorCount.find_first_not_of(" "));

					// If any color count exceeds max, game is impossible
					if (!parseColorCount(colorCount)) {
						return false;
					}
				}
			}

			return true;
		}

		// Calculate sum of possible game IDs
		int sumPossibleGameIDs(const std::string& filename) {
			std::ifstream inputFile(filename);
			if (!inputFile.is_open()) {
				std::cerr << "Error opening file: " << filename << std::endl;
				return -1;
			}

			std::string line;
			int totalSum = 0;

			while (std::getline(inputFile, line)) {
				// Extract game ID
				size_t colonPos = line.find(':');
				int gameID = std::stoi(line.substr(5, colonPos - 5));

				// Add game ID if game is possible
				if (isGamePossible(line)) {
					totalSum += gameID;
				}
			}

			return totalSum;
		}
};

int main() {
	CubeGameValidator validator;

	int result = validator.sumPossibleGameIDs("input.txt");

	if (result != -1) {
		std::cout << "Sum of possible game IDs: " << result << std::endl;
	}

	return 0;
}
