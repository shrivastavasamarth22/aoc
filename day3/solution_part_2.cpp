#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {

	fstream file("input.txt");
	if (!file) {
		cerr << "File not found" << endl;
		return 1;
	}

	string line;
	long long total = 0;

	while(getline(file, line)) {
		bool multiply_enabled = false;
		size_t pos = 0;

		while(pos < line.length()) {
			if (pos + 4 <= line.length() && line.substr(pos, 4) == "do()") {
				multiply_enabled = true;
				pos += 4;
				continue;
			}

			if (pos + 6 <= line.length() && line.substr(pos, 6) == "don't()") {
				multiply_enabled = false;
				pos += 6;
				continue;
			}

			if (pos + 4 <= line.length() && line.substr(pos, 4) == "mul(") {
				size_t end_pos = line.find(")", pos);
				if (end_pos != string::npos) {
					string mul_str = line.substr(pos + 4, end_pos - (pos + 4));
					size_t comma_pos = mul_str.find(",");
					if (comma_pos != string::npos) {
						string num1_str = mul_str.substr(0, comma_pos);
						string num2_str = mul_str.substr(comma_pos + 1);

						bool valid = true;
						for (char c : num1_str) if (!isdigit(c)) valid = false;
						for (char c : num2_str) if (!isdigit(c)) valid = false;
						if (num1_str.length() > 3 || num2_str.length() > 3) valid = false;
						if (num1_str.empty() || num2_str.empty()) valid = false;

						if (valid && multiply_enabled) {
							int x = stoi(num1_str);
							int y = stoi(num2_str);
							total += x * y;
						}
						pos = end_pos + 1;
						continue;
					}
				}
			}
			pos++;
		}
	}

	cout << total << endl;
	return 0;
}
