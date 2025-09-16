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
      // Reserved - should not appear in valid programs
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

  void executeInstruction() {
    if (instructionPointer >= program.size()) {
      return; // Halt
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
        return; // Don't increment IP by 2
      }
      break;

    case 4: // bxc - B XOR C
      regB = regB ^ regC;
      // Operand is ignored for legacy reasons
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

  void run() {
    while (instructionPointer < program.size()) {
      executeInstruction();
    }
  }

  string getOutput() {
    if (output.empty()) {
      return "";
    }

    string result = to_string(output[0]);
    for (int i = 1; i < output.size(); i++) {
      result += "," + to_string(output[i]);
    }
    return result;
  }

  void printState() {
    cout << "Register A: " << regA << endl;
    cout << "Register B: " << regB << endl;
    cout << "Register C: " << regC << endl;
    cout << "Instruction Pointer: " << instructionPointer << endl;

    cout << "Program: ";
    for (int i = 0; i < program.size(); i++) {
      if (i > 0)
        cout << ",";
      cout << program[i];
    }
    cout << endl;

    cout << "Output: " << getOutput() << endl;
    cout << endl;
  }

  // For testing the examples
  void setRegisters(long long a, long long b, long long c) {
    regA = a;
    regB = b;
    regC = c;
  }

  void setProgram(const vector<int> &prog) {
    program = prog;
    instructionPointer = 0;
    output.clear();
  }

  void runTests() {
    cout << "=== Running Tests ===" << endl;

    // Test 1: If register C contains 9, the program 2,6 would set register B to
    // 1
    setRegisters(0, 0, 9);
    setProgram({2, 6});
    run();
    cout << "Test 1 - Register B should be 1: " << regB << endl;

    // Test 2: If register A contains 10, the program 5,0,5,1,5,4 would output
    // 0,1,2
    setRegisters(10, 0, 0);
    setProgram({5, 0, 5, 1, 5, 4});
    run();
    cout << "Test 2 - Output should be 0,1,2: " << getOutput() << endl;

    // Test 3: If register A contains 2024, the program 0,1,5,4,3,0 would output
    // 4,2,5,6,7,7,7,7,3,1,0 and leave 0 in register A
    setRegisters(2024, 0, 0);
    setProgram({0, 1, 5, 4, 3, 0});
    run();
    cout << "Test 3 - Output: " << getOutput()
         << ", Register A should be 0: " << regA << endl;

    // Test 4: If register B contains 29, the program 1,7 would set register B
    // to 26
    setRegisters(0, 29, 0);
    setProgram({1, 7});
    run();
    cout << "Test 4 - Register B should be 26: " << regB << endl;

    // Test 5: If register B contains 2024 and register C contains 43690, the
    // program 4,0 would set register B to 44354
    setRegisters(0, 2024, 43690);
    setProgram({4, 0});
    run();
    cout << "Test 5 - Register B should be 44354: " << regB << endl;

    cout << "=== Tests Complete ===" << endl << endl;
  }
};

int main() {
  ChronospatialComputer computer;

  // Run tests first to verify implementation
  computer.runTests();

  // Load and run the actual program
  cout << "=== Running Main Program ===" << endl;
  computer.loadProgram("input.txt");

  cout << "Initial state:" << endl;
  computer.printState();

  computer.run();

  cout << "Final state:" << endl;
  computer.printState();

  cout << "Final output: " << computer.getOutput() << endl;

  return 0;
}
