#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Function to concatenate two numbers
long long concatenate(long long left, long long right) {
  string leftStr = to_string(left);
  string rightStr = to_string(right);
  return stoll(leftStr + rightStr);
}

// Function to evaluate expression left-to-right with given operators
long long evaluateExpression(const vector<long long> &numbers,
                             const vector<char> &operators) {
  long long result = numbers[0];

  for (size_t i = 0; i < operators.size(); i++) {
    if (operators[i] == '+') {
      result += numbers[i + 1];
    } else if (operators[i] == '*') {
      result *= numbers[i + 1];
    } else if (operators[i] == '|') { // concatenation
      result = concatenate(result, numbers[i + 1]);
    }
  }

  return result;
}

// Recursive function to try all operator combinations
bool canMakeTarget(long long target, const vector<long long> &numbers, int pos,
                   long long current) {
  // Base case: we've processed all numbers
  if (pos == numbers.size()) {
    return current == target;
  }

  // Early termination: if current result is already larger than target for
  // multiplication (this optimization might be too aggressive, so let's be
  // careful)

  // Try addition
  if (canMakeTarget(target, numbers, pos + 1, current + numbers[pos])) {
    return true;
  }

  // Try multiplication
  if (canMakeTarget(target, numbers, pos + 1, current * numbers[pos])) {
    return true;
  }

  // Try concatenation
  long long concatenated = concatenate(current, numbers[pos]);
  if (canMakeTarget(target, numbers, pos + 1, concatenated)) {
    return true;
  }

  return false;
}

int main() {
  ifstream file("input.txt");
  if (!file.is_open()) {
    cerr << "Error: Could not open input.txt" << endl;
    return 1;
  }

  string line;
  long long totalCalibrationResult = 0;
  int validCount = 0;
  int totalCount = 0;

  while (getline(file, line)) {
    totalCount++;

    // Parse the line
    size_t colonPos = line.find(':');
    if (colonPos == string::npos)
      continue;

    long long testValue = stoll(line.substr(0, colonPos));

    string numbersStr = line.substr(colonPos + 1);
    stringstream ss(numbersStr);
    vector<long long> numbers;
    long long num;

    while (ss >> num) {
      numbers.push_back(num);
    }

    if (numbers.empty())
      continue;

    // Check if this equation can be made true using recursive approach
    if (canMakeTarget(testValue, numbers, 1, numbers[0])) {
      totalCalibrationResult += testValue;
      validCount++;
      cout << "Valid equation #" << validCount << ": " << testValue << " = ";
      for (size_t i = 0; i < numbers.size(); i++) {
        cout << numbers[i];
        if (i < numbers.size() - 1)
          cout << " ? ";
      }
      cout << endl;
    }
  }

  file.close();

  cout << "\nProcessed " << totalCount << " equations" << endl;
  cout << "Found " << validCount << " valid equations" << endl;
  cout << "Total calibration result: " << totalCalibrationResult << endl;

  return 0;
}
