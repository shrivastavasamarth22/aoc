#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

int extrapolateNextValue(const vector<int>& sequence) {
	bool allZero = true;
	for (int num : sequence) {
		if (num != 0) {
			allZero = false;
			break;
		}
	}

	if (allZero) return 0;

	vector<int> diffSequence;
	for (size_t i = 1; i < sequence.size(); i++) {
		diffSequence.push_back(sequence[i] - sequence[i - 1]);
	}

	int extrapolateDiff = extrapolateNextValue(diffSequence);

	return sequence.back() + extrapolateDiff;
}

int main() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		cerr << "Error: could not open file" << endl;
		return 1;
	}

	string line;
	long long totalSum = 0;

	while(getline(file, line)) {
		vector<int> sequence;
		istringstream iss(line);
		int num;

		while (iss >> num) {
			sequence.push_back(num);
		}

		int nextValue = extrapolateNextValue(sequence);
		totalSum += nextValue;
	}

	cout << "Sum of extrapolated values: " << totalSum << endl;

	return 0;
}
