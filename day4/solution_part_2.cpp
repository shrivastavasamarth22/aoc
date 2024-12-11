#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

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

		int calculateCardMatches(const string& card) {
			size_t colonPos = card.find(":");
			size_t barPos = card.find("|");
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
			return matchCount;
		}

	public:
		int calculateTotalScratchcards(const string& filename) {
			ifstream file(filename);
			if (!file.is_open()) {
				cerr << "Error: cannot open file " << filename << endl;
				return -1;
			}

			vector<string> cards;
			string line;

			while(getline(file, line)) {
				cards.push_back(line);
			}

			vector<int> cardCopies(cards.size(), 1);

			for (size_t i = 0; i < cards.size(); i++) {
				int matches = calculateCardMatches(cards[i]);

				for (int j = 1; j <= matches && i + j < cards.size(); j++) {
					cardCopies[i + j] += cardCopies[i];
				}
			}
			
			int totalCards = 0;
			for (int copies : cardCopies) {
				totalCards += copies;
			}

			return totalCards;

		}
};

int main() {
	ScratchCardCalculator calculator;
	
	int result = calculator.calculateTotalScratchcards("input.txt");

	if (result != -1) {
		cout << "Total scratchcards: " << result << endl;
	}

	return 0;
}
