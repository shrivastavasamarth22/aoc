#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

int main() {
	ifstream inFile("input.txt");
	if (!inFile) {
		cerr << "Cannot open input.txt\n";
		return 1;
	}

	string line;
	long long total = 0;

	// Regular expressions for different patterns
	regex mul_pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");
	regex do_pattern(R"(do\(\))");
	regex dont_pattern(R"(don't\(\))");

	while (getline(inFile, line)) {
		bool multiply_enabled = true;  // Start with multiplication enabled
		size_t current_pos = 0;

		while (current_pos < line.length()) {
			// Try to match patterns at the current position
			string remaining = line.substr(current_pos);

			// Check for do() instruction
			smatch do_match;
			if (regex_search(remaining, do_match, do_pattern) && do_match.position() == 0) {
				multiply_enabled = true;
				current_pos += do_match.length();
				continue;
			}

			// Check for don't() instruction
			smatch dont_match;
			if (regex_search(remaining, dont_match, dont_pattern) && dont_match.position() == 0) {
				multiply_enabled = false;
				current_pos += dont_match.length();
				continue;
			}

			// Check for mul instruction
			smatch mul_match;
			if (regex_search(remaining, mul_match, mul_pattern) && mul_match.position() == 0) {
				if (multiply_enabled) {
					int x = stoi(mul_match[1]);
					int y = stoi(mul_match[2]);
					total += x * y;
				}
				current_pos += mul_match.length();
				continue;
			}

			// Move to next character if no pattern matches
			current_pos++;
		}
	}

	cout << "Sum of enabled multiplications: " << total << endl;

	return 0;
}
