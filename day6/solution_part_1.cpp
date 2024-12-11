#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class BoatRaceCalculator {
	private:
		vector<long long> times;
		vector<long long> distances;

		long long calculateWinningWays(long long time, long long recordDistance) {
			long long winningWays = 0;

			for (long long holdTime = 0; holdTime <= time; holdTime++) {
				long long remainingTime = time - holdTime;
				long long travelDistance = holdTime * remainingTime;

				if (travelDistance > recordDistance) {
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
			vector<long long>* currentVector = nullptr;

			while (getline(file, line)) {
				istringstream iss(line);
				string label;
				iss >> label;

				if (label == "Time:") {
					currentVector = &times;
				} else if (label == "Distance:") {
					currentVector = &distances;
				}

				if (currentVector) {
					long long value;
					while (iss >> value) {
						currentVector->push_back(value);
					}
				}
			}
		}

		long long calculateTotalWinningWays() {
			long long totalWays = 1;

			for (size_t i = 0; i < times.size(); i++) {
				long long waysToWin = calculateWinningWays(times[i], distances[i]);
				totalWays *= waysToWin;
			}

			return totalWays;
		}
};

int main() {
	BoatRaceCalculator calculator;
	calculator.parseInput("input.txt");

	long long result = calculator.calculateTotalWinningWays();
	cout << "Total winning ways: " << result << endl;

	return 0;
}
