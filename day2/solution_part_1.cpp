#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

bool isReportSafe(const vector<int>& levels) {
	if (levels.size() < 2) return true;

	bool shouldBeIncreasing = (levels[1] > levels[0]);

	for (size_t i = 1; i < levels.size(); i++) {
		int diff = levels[i] - levels[i - 1];

		if (abs(diff) < 1 || abs(diff) > 3) return false;

		if (shouldBeIncreasing && diff <= 0) return false;

		if (!shouldBeIncreasing && diff >= 0) return false;

	}

	return true;
}

int main() {
	ifstream file("input.txt");
	if (!file) {
		cerr << "File not found" << endl;
		return 1;
	}

	int safeCount = 0;
	string line;

	while(getline(file, line)) {
		vector<int> levels;
		stringstream ss(line);
		int level;

		while(ss >> level) {
			levels.push_back(level);
		}

		if (isReportSafe(levels)) {
			safeCount++;
		}
	}

	cout << "Number of safe reports: " << safeCount << endl;

	return 0;
}
