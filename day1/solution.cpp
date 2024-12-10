#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

int processLine(const string& line) {
	// Find first and last digit
	int first = -1, last = -1;

	for (char c : line) {
		if (isdigit(c)) {
			// If first digit is not found yet, set it to the current digit
			if (first == -1) {
				first = c - '0';
			}
			// Always update last digit when a digit is found
			last = c - '0';
		}
	}

	// Combine first and last digit to form a two digit number
	return first * 10 + last;
}


int main() {
	ifstream file("input.txt");

	if (!file.is_open()) {
		cerr << "File not found" << endl;
		return 1;
	}

	string line;
	int sum = 0;

	while(getline(file, line)) {
		sum += processLine(line);
	}

	file.close();

	cout << sum << endl;

	return 0;
}

