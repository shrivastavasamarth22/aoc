#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int countDigits(long long num) {
  if (num == 0)
    return 1;
  int count = 0;
  while (num > 0) {
    count++;
    num /= 10;
  }
  return count;
}

pair<long long, long long> splitNumber(long long num) {
  string numStr = to_string(num);
  int len = numStr.length();
  int mid = len / 2;

  string leftStr = numStr.substr(0, mid);
  string rightStr = numStr.substr(mid);

  long long left = stoll(leftStr);
  long long right = stoll(rightStr);

  return make_pair(left, right);
}

vector<long long> performBlink(const vector<long long> &stones) {
  vector<long long> newStones;

  for (long long stone : stones) {
    if (stone == 0) {
      newStones.push_back(1);
    } else if (countDigits(stone) % 2 == 0) {
      pair<long long, long long> split = splitNumber(stone);
      newStones.push_back(split.first);
      newStones.push_back(split.second);
    } else {
      newStones.push_back(stone * 2024);
    }
  }

  return newStones;
}

void printStones(const vector<long long> &stones, int maxPrint = 50) {
  for (size_t i = 0; i < min(stones.size(), (size_t)maxPrint); i++) {
    cout << stones[i];
    if (i < stones.size() - 1)
      cout << " ";
  }
  if (stones.size() > maxPrint) {
    cout << " ... (" << stones.size() << " total stones)";
  }
  cout << endl;
}

int main() {
  ifstream file("input.txt");
  if (!file.is_open()) {
    cerr << "Error: Could not open input.txt" << endl;
    return 1;
  }

  string line;
  getline(file, line);
  file.close();

  vector<long long> stones;
  stringstream ss(line);
  long long stone;

  while (ss >> stone) {
    stones.push_back(stone);
  }

  cout << "Initial arrangement (" << stones.size() << " stones):" << endl;
  printStones(stones);
  cout << endl;

  // Perform 25 blinks
  for (int blink = 1; blink <= 25; blink++) {
    stones = performBlink(stones);

    cout << "After " << blink << " blink" << (blink == 1 ? "" : "s") << " ("
         << stones.size() << " stones):" << endl;

    if (blink <= 10 ||
        blink % 5 == 0) { // Show detailed output for first 10 and every 5th
      printStones(stones, 30);
    } else {
      cout << "..." << stones.size() << " stones total" << endl;
    }
    cout << endl;
  }

  cout << "Final result: " << stones.size() << " stones after 25 blinks"
       << endl;

  return 0;
}
