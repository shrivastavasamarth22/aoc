#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

int extrapolatePreviousValue(const vector<int>& sequence) {
	bool allZero = true;
	for (int num : sequence) {
		if (num != 0) {
			allZero = false;
			break;
		}
	}
	if (allZero) return 0;

	vector<int> diffSequence;
	for (size_t i = 1; i < sequence.size(); ++i) {
		diffSequence.push_back(sequence[i] - sequence[i-1]);
	}

	int extrapolatedDiff = extrapolatePreviousValue(diffSequence);

	return sequence.front() - extrapolatedDiff;
}

int main() {
	ifstream inputFile("input.txt");
	if (!inputFile) {
		cerr << "Unable to open input file" << endl;
		return 1;
	}

	string line;
	long long previousValueSum = 0;

	while (getline(inputFile, line)) {
		vector<int> sequence;
		istringstream iss(line);
		int num;

		while (iss >> num) {
			sequence.push_back(num);
		}

		int previousValue = extrapolatePreviousValue(sequence);

		previousValueSum += previousValue;
	}

	cout << "Sum of extrapolated previous values: " << previousValueSum << endl;

	return 0;
}
