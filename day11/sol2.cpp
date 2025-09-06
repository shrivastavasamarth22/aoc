#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

map<pair<long long, int>, long long> memo;

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

long long countStonesAfterBlinks(long long stone, int blinks) {
  if (blinks == 0) {
    return 1;
  }

  pair<long long, int> key = make_pair(stone, blinks);
  if (memo.find(key) != memo.end()) {
    return memo[key];
  }

  long long result;

  if (stone == 0) {
    result = countStonesAfterBlinks(1, blinks - 1);
  } else if (countDigits(stone) % 2 == 0) {
    pair<long long, long long> split = splitNumber(stone);
    result = countStonesAfterBlinks(split.first, blinks - 1) +
             countStonesAfterBlinks(split.second, blinks - 1);
  } else {
    result = countStonesAfterBlinks(stone * 2024, blinks - 1);
  }

  memo[key] = result;
  return result;
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

  // Parse the initial stone arrangement
  vector<long long> stones;
  stringstream ss(line);
  long long stone;

  while (ss >> stone) {
    stones.push_back(stone);
  }

  cout << "Initial arrangement (" << stones.size() << " stones): ";
  for (size_t i = 0; i < stones.size(); i++) {
    cout << stones[i];
    if (i < stones.size() - 1)
      cout << " ";
  }
  cout << endl << endl;

  // Calculate for both Part 1 (25 blinks) and Part 2 (75 blinks)
  for (int targetBlinks : {25, 75}) {
    long long totalStones = 0;

    cout << "Calculating for " << targetBlinks << " blinks..." << endl;

    for (size_t i = 0; i < stones.size(); i++) {
      long long stoneCount = countStonesAfterBlinks(stones[i], targetBlinks);
      cout << "Stone " << stones[i] << " produces " << stoneCount << " stones"
           << endl;
      totalStones += stoneCount;
    }

    cout << "Total stones after " << targetBlinks << " blinks: " << totalStones
         << endl;
    cout << "Memo cache size: " << memo.size() << " entries" << endl << endl;
  }

  return 0;
}
