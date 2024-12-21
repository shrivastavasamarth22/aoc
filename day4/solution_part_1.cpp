#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

bool checkWord(const vector<string>& grid, int x, int y, int dir, const string& word) {
	int rows = grid.size();
	int cols = grid[0].size();

	if (x + dx[dir] * (word.length() - 1) < 0 || x + dx[dir] * (word.length() - 1) >= rows) return false;
	if (y + dy[dir] * (word.length() - 1) < 0 || y + dy[dir] * (word.length() - 1) >= cols) return false;

	for (size_t i = 0; i < word.length(); i++) {
		if (grid[x + dx[dir] * i][y + dy[dir] * i] != word[i]) return false;
	}
	return true;
}

int countOccurences(const vector<string>& grid, const string& word) {
	int count = 0;
	int rows = grid.size();
	int cols = grid[0].size();

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			for (int dir = 0; dir < 8; dir++) {
				if (checkWord(grid, i, j, dir, word)) {
					count++;
				}
			}
		}
	}

	return count;
}

int main() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		cerr << "File not found!" << endl;
		return 1;
	}

	vector<string> grid;
	string line;

	while(getline(file, line)) {
		grid.push_back(line);
	}

	int result  = countOccurences(grid, "XMAS");

	cout << "XMAS appears " << result << " times in the word search grid." << endl;

	return 0;
}

