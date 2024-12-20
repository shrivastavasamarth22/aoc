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

bool isReportSafeWithDampener(const vector<int>& levels) {
	if (isReportSafe(levels)) return true;

	for (size_t i = 0; i < levels.size(); i++) {
		vector<int> modifiedLevels;

		for(size_t j = 0; j < levels.size(); j++) {
			if (j != i) {
				modifiedLevels.push_back(levels[j]);
			}
		}

		if (isReportSafe(modifiedLevels)) return true;

	}
	return false;
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

		if (isReportSafeWithDampener(levels)) {
			safeCount++;
		}
	}

	cout << "Number of safe reports: " << safeCount << endl;
	return 0;
}
