#include <fstream>
#include <iostream>
#include <regex>

using namespace std;

int main() {
  ifstream file("input.txt");
  string content;
  string line;

  while (getline(file, line)) {
    content += line;
  }
  file.close();

  // Regex pattern to match mul(X,Y), do(), and don't()
  regex pattern(R"(mul\((\d{1,3}),(\d{1,3})\)|do\(\)|don't\(\))");

  bool enabled = true;
  long long total = 0;

  // Find all matches
  sregex_iterator iter(content.begin(), content.end(), pattern);
  sregex_iterator end;

  for (; iter != end; ++iter) {
    const smatch &match = *iter;
    string matched = match.str();

    if (matched == "do()") {
      enabled = true;
    } else if (matched == "don't()") {
      enabled = false;
    } else if (matched.substr(0, 3) == "mul" && enabled) {
      // Extract the two numbers
      int x = stoi(match[1].str());
      int y = stoi(match[2].str());
      total += (long long)x * y;
    }
  }

  cout << "Total: " << total << endl;

  return 0;
}
