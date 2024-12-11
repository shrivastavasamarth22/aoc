#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

class AlmanacMapper {
	private:
		struct MapRange {
			long long destStart;
			long long sourceStart;
			long long length;
		};

		vector<long long> seeds;
		vector<vector<MapRange>> maps;

		long long mapValue(long long value, const vector<MapRange>& currentMap) {
			for (const auto& range : currentMap) {
				if (value >= range.sourceStart && value < range.sourceStart + range.length) {
					return range.destStart + (value - range.sourceStart);
				}
			}
			return value;
		}

	public:
		void parseInput(const string& filename) {
			ifstream file(filename);
			if (!file.is_open()) {
				cerr << "Could not open the file!" << endl;
				return;
			}

			string line;

			getline(file, line);
			istringstream seadStream(line.substr(line.find(":") + 1));
			long long seed;
			while (seadStream >> seed) {
				seeds.push_back(seed);
			}

			vector<MapRange> currentMap;
			while(getline(file, line)) {
				if (line.empty()) continue;

				if (line.find("map:") != string::npos) {
					if (!currentMap.empty()) {
						maps.push_back(currentMap);
						currentMap.clear();
					}
					continue;
				}
				MapRange range;
				istringstream rangeStream(line);
				rangeStream >> range.destStart >> range.sourceStart >> range.length;
				currentMap.push_back(range);
			}
			if (!currentMap.empty()) {
				maps.push_back(currentMap);
			}
		}

		long long findLowestLocation() {
			long long lowestLocation = numeric_limits<long long>::max();

			for (long long seed : seeds) {
				long long currentValue = seed;

				for (const auto& currentMap : maps) {
					currentValue =  mapValue(currentValue, currentMap);
				}

				lowestLocation = min(lowestLocation, currentValue);
			}
			return lowestLocation;
		}
};

int main() {
	AlmanacMapper mapper;
	mapper.parseInput("input.txt");

	long long result = mapper.findLowestLocation();
	cout << "The lowest location is: " << result << endl;

	return 0;
}
