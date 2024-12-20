#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

using namespace std;

int main() {
	ifstream file("input.txt");
	if (!file) {
		cerr << "File not found" << endl;
		return 1;
	}

	vector<int> leftList, rightList;
	unordered_map<int, int> rightCounts;
	int left, right;

	while(file >> left >> right) {
		leftList.push_back(left);
		rightCounts[right]++;
	}

	long long similarityScore = 0;
	
	for (int num: leftList) {
		similarityScore += (long long)num * rightCounts[num];
	}

	cout << "Similarity score: " << similarityScore << endl;

	return 0;
}
