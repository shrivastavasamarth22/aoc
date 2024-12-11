#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

struct MapRange {
	long long destStart;
	long long sourceStart;
	long long length;
};

struct SeedRange {
	long long start;
	long long length;
};

class AlmanacMapper {
	private:
		vector<SeedRange> seedRanges;
		vector<vector<MapRange>> maps;

		vector<SeedRange> transformRanges(const vector<SeedRange>& inputRanges, const vector<MapRange>& currentMap) {
			vector<SeedRange> outputRanges;

			for(const auto& seedRange : inputRanges) {
				vector<SeedRange> remainingRanges = {seedRange};

				for(const auto& mapRange : currentMap) {
					vector<SeedRange> newRemainingRanges;

					for(const auto& currentRange : remainingRanges) {
						long long rangeStart = currentRange.start;
						long long rangeEnd = currentRange.start + currentRange.length - 1;
						long long mapStart = mapRange.sourceStart;
						long long mapEnd = mapRange.sourceStart + mapRange.length - 1;
						if (rangeEnd < mapStart || rangeStart > mapEnd) {
							newRemainingRanges.push_back(currentRange);
							continue;
						}

						long long overlapStart = max(rangeStart, mapStart);
						long long overlapEnd = min(rangeEnd, mapEnd);
						long long overlapLength = overlapEnd - overlapStart + 1;

						SeedRange transformedRange = {
							mapRange.destStart + (overlapStart - mapRange.sourceStart),
							overlapLength
						};
						outputRanges.push_back(transformedRange);

						if (rangeEnd > overlapEnd) {
							newRemainingRanges.push_back({
								overlapEnd + 1,
								rangeEnd - overlapEnd
							});
						}
				}
					remainingRanges = newRemainingRanges;
				}
				outputRanges.insert(
					outputRanges.end(),
					remainingRanges.begin(),
					remainingRanges.end()
				);
			}
			return outputRanges;
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
			istringstream seedStream(line.substr(line.find(":") + 1));
			long long start, length;
			while(seedStream >> start >> length) {
				seedRanges.push_back({start, length});
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
			vector<SeedRange> currentRanges = seedRanges;

			for (const auto& currentMap : maps) {
				currentRanges = transformRanges(currentRanges, currentMap);
			}

			long long lowestLocation = numeric_limits<long long>::max();
			for (const auto& range : currentRanges) {
				lowestLocation = min(lowestLocation, range.start);
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
