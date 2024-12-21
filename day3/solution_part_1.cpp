#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

int main() {
	ifstream file("input.txt");
	if (!file) {
		cerr << "File not found" << endl;
		return 1;
	}

	string line;
	long long total = 0;

	regex pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");

	while(getline(file, line)) {
		auto begin = sregex_iterator(line.begin(), line.end(), pattern);
		auto end = sregex_iterator();

		for (sregex_iterator i = begin; i != end; ++i) {
			smatch match = *i;
			int x = stoi(match[1]);
			int y = stoi(match[2]);
			total += x * y;
		}
	}

	cout << total << endl;
	return 0;
}
