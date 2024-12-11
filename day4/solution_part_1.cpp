#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <cmath>

using namespace std;

class ScratchCardCalculator {
	private:
		unordered_set<int> parseNumberList(const string& numberList) {
			unordered_set<int> numbers;
			istringstream iss(numberList);
			int num;

			while (iss >> num) {
				numbers.insert(num);
			}

			return numbers;
		}

		int calculateCardPoints(const string& card) {
			size_t colonPos = card.find(':');
			size_t barPos = card.find('|');

			if (colonPos == string::npos || barPos == string::npos) {
				return 0;
			}

			string winningNumbersStr = card.substr(colonPos + 1, barPos - colonPos - 1);
			string haveNumberStr = card.substr(barPos + 1);

			unordered_set<int> winningNumbers = parseNumberList(winningNumbersStr);
			unordered_set<int> haveNumbers = parseNumberList(haveNumberStr);

			int matchCount = 0;
			for (int num : haveNumbers) {
				if (winningNumbers.count(num) > 0) {
					matchCount++;
				}
			}
			return matchCount > 0 ? pow(2, matchCount - 1) : 0;
		}

	public:
		int calculateTotalPoints(const string& filename) {
			ifstream file(filename);
			if (!file.is_open()) {
				cerr << "Error: cannot open file " << filename << endl;
				return -1;
			}

			string line;
			int totalPoints = 0;

			while(getline(file, line)) {
				totalPoints += calculateCardPoints(line);
			}

			return totalPoints;
		}
};

int main() {
	ScratchCardCalculator calculator;
	
	int result = calculator.calculateTotalPoints("input.txt");

	if (result != -1) {
		cout << "Total points: " << result << endl;
	}

	return 0;
}
