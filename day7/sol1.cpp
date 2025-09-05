#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Function to evaluate expression left-to-right with given operators
long long evaluateExpression(const vector<int> &numbers,
                             const vector<char> &operators) {
  long long result = numbers[0];

  for (int i = 0; i < operators.size(); i++) {
    if (operators[i] == '+') {
      result += numbers[i + 1];
    } else if (operators[i] == '*') {
      result *= numbers[i + 1];
    }
  }

  return result;
}

// Function to generate all possible operator combinations
bool canMakeTarget(long long target, const vector<int> &numbers) {
  int numOperators = numbers.size() - 1;

  // Try all possible combinations of operators (2^numOperators combinations)
  for (int mask = 0; mask < (1 << numOperators); mask++) {
    vector<char> operators;

    // Generate operator combination based on bit mask
    for (int i = 0; i < numOperators; i++) {
      if (mask & (1 << i)) {
        operators.push_back('*');
      } else {
        operators.push_back('+');
      }
    }

    // Evaluate the expression with these operators
    long long result = evaluateExpression(numbers, operators);

    if (result == target) {
      return true;
    }
  }

  return false;
}

int main() {
  ifstream file("input.txt");
  string line;
  long long totalCalibrationResult = 0;

  while (getline(file, line)) {
    size_t colonPos = line.find(':');
    long long testValue = stoll(line.substr(0, colonPos));

    string numberStr = line.substr(colonPos + 1);
    stringstream ss(numberStr);
    vector<int> numbers;
    int num;

    while (ss >> num) {
      numbers.push_back(num);
    }

    if (canMakeTarget(testValue, numbers)) {
      totalCalibrationResult += testValue;
      cout << "Valid equation: " << testValue << " with numbers: ";
      for (int n : numbers) {
        cout << n << " ";
      }
      cout << endl;
    }
  }

  file.close();
  cout << "Total calibration result: " << totalCalibrationResult << endl;
  return 0;
}
