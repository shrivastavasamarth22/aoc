#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

int main() {
	ifstream file("input.txt");
	if (!file) {
		cerr << "File not found" << endl;
		return 1;
	}

	vector<int> leftList, rightList;
	int left, right;

	while(file >> left >> right) {
		leftList.push_back(left);
		rightList.push_back(right);
	}

	sort(leftList.begin(), leftList.end());
	sort(rightList.begin(), rightList.end());

	long long totalDistance = 0;

	for(size_t i = 0; i < leftList.size(); i++) {
		totalDistance += abs(leftList[i] - rightList[i]);
	}

	cout << "Total distance: " << totalDistance << endl;

	return 0;
}
