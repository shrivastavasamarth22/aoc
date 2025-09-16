#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class ChronospatialComputer {
private:
  long long regA, regB, regC;
  vector<int> program;
  int instructionPointer;
  vector<int> output;

  long long getComboValue(int operand) {
    switch (operand) {
    case 0:
    case 1:
    case 2:
    case 3:
      return operand; // Literal values 0-3
    case 4:
      return regA;
    case 5:
      return regB;
    case 6:
      return regC;
    case 7:
      throw runtime_error("Invalid combo operand 7");
    default:
      throw runtime_error("Invalid combo operand");
    }
  }

public:
  void loadProgram(const string &filename) {
    ifstream file(filename);
    string line;

    // Read register A
    getline(file, line);
    regA = stoll(line.substr(line.find(": ") + 2));

    // Read register B
    getline(file, line);
    regB = stoll(line.substr(line.find(": ") + 2));

    // Read register C
    getline(file, line);
    regC = stoll(line.substr(line.find(": ") + 2));

    // Skip empty line
    getline(file, line);

    // Read program
    getline(file, line);
    string programStr = line.substr(line.find(": ") + 2);

    // Parse comma-separated program
    stringstream ss(programStr);
    string token;
    while (getline(ss, token, ',')) {
      program.push_back(stoi(token));
    }

    instructionPointer = 0;
    output.clear();
  }

  vector<int> runProgram(long long startA, long long startB = 0,
                         long long startC = 0) {
    regA = startA;
    regB = startB;
    regC = startC;
    instructionPointer = 0;
    output.clear();

    while (instructionPointer < (int)program.size()) {
      if (instructionPointer + 1 >= (int)program.size()) {
        break;
      }

      int opcode = program[instructionPointer];
      int operand = program[instructionPointer + 1];

      switch (opcode) {
      case 0: // adv - A division
        regA = regA / (1LL << getComboValue(operand));
        break;

      case 1: // bxl - B XOR literal
        regB = regB ^ operand;
        break;

      case 2: // bst - combo mod 8 to B
        regB = getComboValue(operand) % 8;
        break;

      case 3: // jnz - jump if A not zero
        if (regA != 0) {
          instructionPointer = operand;
          continue; // Don't increment IP by 2
        }
        break;

      case 4: // bxc - B XOR C
        regB = regB ^ regC;
        break;

      case 5: // out - output combo mod 8
        output.push_back(getComboValue(operand) % 8);
        break;

      case 6: // bdv - A division to B
        regB = regA / (1LL << getComboValue(operand));
        break;

      case 7: // cdv - A division to C
        regC = regA / (1LL << getComboValue(operand));
        break;

      default:
        throw runtime_error("Invalid opcode: " + to_string(opcode));
      }

      instructionPointer += 2;
    }

    return output;
  }

  vector<int> getProgram() const { return program; }

  long long getRegA() const { return regA; }
  long long getRegB() const { return regB; }
  long long getRegC() const { return regC; }

  // Backtracking approach to find self-replicating A
  long long findSelfReplicatingA() {
    vector<int> target = program;
    long long result = backtrack(target, 0, 0);
    return result;
  }

private:
  long long backtrack(const vector<int> &remainingOutput, long long currentA,
                      int depth) {
    if (remainingOutput.empty()) {
      // We've matched all output, test this A value
      vector<int> testOutput = runProgram(currentA, regB, regC);
      if (testOutput == program) {
        return currentA;
      }
      return -1;
    }

    // Get the last digit we need to produce
    int lastDigit = remainingOutput.back();
    vector<int> remaining(remainingOutput.begin(), remainingOutput.end() - 1);

    // Try different values that could produce this digit
    // Most programs work by outputting A % 8, then doing A /= 8
    for (int offset = 0; offset < 8; offset++) {
      long long nextA = currentA * 8 + offset;

      if (nextA == 0 && !remainingOutput.empty()) {
        continue; // Skip if we still have output to produce
      }

      // Quick test: run the program and check if first output matches
      vector<int> testOutput = runProgram(nextA, regB, regC);

      if (!testOutput.empty() && testOutput[0] == lastDigit) {
        // This looks promising, recurse
        long long result = backtrack(remaining, nextA, depth + 1);
        if (result != -1) {
          return result;
        }
      }
    }

    return -1; // No valid A found for this branch
  }

public:
  // Alternative brute force approach for verification or fallback
  long long findSelfReplicatingBruteForce(long long maxSearch = 10000000) {
    cout << "Using brute force search up to " << maxSearch << "..." << endl;

    for (long long a = 1; a <= maxSearch; a++) {
      if (a % 100000 == 0) {
        cout << "Checking A = " << a << "..." << endl;
      }

      vector<int> output = runProgram(a, regB, regC);
      if (output == program) {
        return a;
      }
    }

    return -1; // Not found
  }

  void printProgram() {
    cout << "Program: ";
    for (size_t i = 0; i < program.size(); i++) {
      if (i > 0)
        cout << ",";
      cout << program[i];
    }
    cout << endl;
  }

  void testExample() {
    cout << "=== Testing Example ===" << endl;

    // Test the example: Program [0,3,5,4,3,0] with A=117440
    program = {0, 3, 5, 4, 3, 0};
    regB = 0;
    regC = 0;

    cout << "Example program: ";
    printProgram();

    // Test known working value
    vector<int> output = runProgram(117440, 0, 0);
    cout << "A=117440 produces output: ";
    for (size_t i = 0; i < output.size(); i++) {
      if (i > 0)
        cout << ",";
      cout << output[i];
    }
    cout << endl;

    cout << "Expected: ";
    for (size_t i = 0; i < program.size(); i++) {
      if (i > 0)
        cout << ",";
      cout << program[i];
    }
    cout << endl;

    cout << "Match: " << (output == program ? "Yes" : "No") << endl;

    // Find minimum A
    long long minA = findSelfReplicatingA();
    if (minA != -1) {
      cout << "Minimum A found: " << minA << endl;

      // Verify
      vector<int> verifyOutput = runProgram(minA, 0, 0);
      cout << "Verification match: " << (verifyOutput == program ? "Yes" : "No")
           << endl;
    } else {
      cout
          << "Could not find minimum A with backtracking, trying brute force..."
          << endl;
      minA = findSelfReplicatingBruteForce(1000000);
      if (minA != -1) {
        cout << "Minimum A found with brute force: " << minA << endl;
      } else {
        cout << "Could not find minimum A" << endl;
      }
    }

    cout << endl;
  }
};

int main() {
  ChronospatialComputer computer;

  // Test with example first
  computer.testExample();

  // Load and solve the actual problem
  cout << "=== Solving Main Problem ===" << endl;

  try {
    computer.loadProgram("input.txt");

    cout << "Loaded program from input.txt:" << endl;
    cout << "Initial registers: A=" << computer.getRegA()
         << ", B=" << computer.getRegB() << ", C=" << computer.getRegC()
         << endl;
    computer.printProgram();
    cout << endl;

    cout << "Searching for self-replicating A value using backtracking..."
         << endl;
    long long result = computer.findSelfReplicatingA();

    if (result != -1) {
      cout << "Found self-replicating A value: " << result << endl;

      // Verify the result
      vector<int> output =
          computer.runProgram(result, computer.getRegB(), computer.getRegC());
      vector<int> target = computer.getProgram();

      cout << "Verification:" << endl;
      cout << "Output:  ";
      for (size_t i = 0; i < output.size(); i++) {
        if (i > 0)
          cout << ",";
        cout << output[i];
      }
      cout << endl;

      cout << "Target:  ";
      for (size_t i = 0; i < target.size(); i++) {
        if (i > 0)
          cout << ",";
        cout << target[i];
      }
      cout << endl;

      cout << "Match: " << (output == target ? "Yes" : "No") << endl;
      cout << endl;
      cout << "Answer: " << result << endl;

    } else {
      cout << "Backtracking failed, trying brute force..." << endl;
      result = computer.findSelfReplicatingBruteForce();

      if (result != -1) {
        cout << "Found self-replicating A value with brute force: " << result
             << endl;
        cout << "Answer: " << result << endl;
      } else {
        cout << "Could not find self-replicating A value" << endl;
      }
    }

  } catch (const exception &e) {
    cout << "Error: " << e.what() << endl;
    return 1;
  }

  return 0;
}
