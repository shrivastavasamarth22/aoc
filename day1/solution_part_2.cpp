#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <unordered_map>
#include <limits>

using namespace std;

class CalibrationDecoder {
	private:
		unordered_map<std::string, int> wordToDigit = {
			{"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, 
			{"five", 5}, {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}
		};
		// Find first digit/word-digit in the line
		int findFirstDigit(const string& line) {
			int minIndex = numeric_limits<int>::max();
			int firstDigit = -1;

			// Check numeric digits
			for (int i = 0; i < line.length(); ++i) {
				if (isdigit(line[i])) {
					minIndex = i;
					firstDigit = line[i] - '0';
					break;
				}
			}

			// Check word digits
			for (const auto& [word, digit] : wordToDigit) {
				size_t pos = line.find(word);
				if (pos != string::npos && pos < minIndex) {
					minIndex = pos;
					firstDigit = digit;
				}
			}
			return firstDigit;
		}

		// Find last digit/word-digit in the line
		int findLastDigit(const string& line) {
			int maxIndex = -1;
			int lastDigit = -1;

			for (int i = 0; i < line.length(); ++i) {
				if (isdigit(line[i])) {
					maxIndex = i;
					lastDigit = line[i] - '0';
				}
			}

			// Check word digits
			for (const auto& [word, digit] : wordToDigit) {
				size_t pos = line.rfind(word);
				if (pos != string::npos && pos > maxIndex) {
					maxIndex = pos;
					lastDigit = digit;
				}
			}
			return lastDigit;
		}

	public:
		int processLine(const string& line) {
			int first = findFirstDigit(line);
			int last = findLastDigit(line);
			return first * 10 + last;
		}

		int calculateTotalSum(const string& filename) {
			ifstream file(filename);

			if (!file.is_open()) {
				cerr << "Error: cannot open file " << filename << endl;
				return -1;
			}

			std::string line;
			int totalSum = 0;

			while (getline(file, line)) {
				totalSum += processLine(line);
			}
			file.close();
			return totalSum;
		}
};

int main() {
	CalibrationDecoder decoder;
	int result = decoder.calculateTotalSum("input.txt");

	if (result != -1) {
		cout << "Total sum: " << result << endl;
	}

	return 0;
}


