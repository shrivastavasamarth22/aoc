#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class BoatRaceCalculator {
	private:
		long long time;
		long long distance;

		long long calculateWinningWays() {
			long long winningWays = 0;

			for (long long holdTime = 0; holdTime <= time; holdTime++) {
				long long remainingTime = time - holdTime;
				long long travelDistance = holdTime * remainingTime;

				if (travelDistance > distance) {
					winningWays++;
				}
			}

			return winningWays;
		}

	public:
		void parseInput(const string& filename) {
			ifstream file(filename);
			if (!file.is_open()) {
				cerr << "Error opening file " << filename << endl;
				return;
			}

			string line;
			string timeStr, distanceStr;

			getline(file, line);
			size_t timePos = line.find(":");
			timeStr = line.substr(timePos + 1);

			getline(file, line);
			size_t distancePos = line.find(":");
			distanceStr = line.substr(distancePos + 1);

			timeStr.erase(remove(timeStr.begin(), timeStr.end(), ' '), timeStr.end());
			distanceStr.erase(remove(distanceStr.begin(), distanceStr.end(), ' '), distanceStr.end());

			time = stoll(timeStr);
			distance = stoll(distanceStr);
		}

		long long calculateTotalWinningWays() {
			return calculateWinningWays();
		}
};

int main() {
	BoatRaceCalculator calculator;
	calculator.parseInput("input.txt");
	cout << calculator.calculateTotalWinningWays() << endl;

	return 0;
}
