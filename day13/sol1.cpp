#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <utility>
#include <vector>

using namespace std;

struct Machine {
  long long ax, ay;
  long long bx, by;
  long long px, py;
};

pair<long long, long long> solveMachine(const Machine &machine) {
  long long ax = machine.ax, ay = machine.ay;
  long long bx = machine.bx, by = machine.by;
  long long px = machine.px, py = machine.py;

  long long det = ax * by - ay * bx;

  if (det == 0) {
    return make_pair(-1, -1);
  }

  long long a_num = px * by - py * bx;
  long long b_num = ax * py - ay * px;

  if (a_num % det != 0 || b_num % det != 0) {
    return make_pair(-1, -1);
  }

  long long a = a_num / det;
  long long b = b_num / det;

  if (a >= 0 && b >= 0 && a <= 100 && b <= 100) {
    return make_pair(a, b);
  }
  return make_pair(-1, -1);
}

int main() {
  ifstream file("input.txt");

  vector<Machine> machines;
  string line;

  while (getline(file, line)) {
    if (line.empty())
      continue;

    Machine machine;

    if (line.find("Button A:") != string::npos) {
      regex buttonA_regex(R"(Button A: X\+(\d+), Y\+(\d+))");
      smatch match;
      if (regex_match(line, match, buttonA_regex)) {
        machine.ax = stoll(match[1]);
        machine.ay = stoll(match[2]);
      }

      getline(file, line);
      regex buttonB_regex(R"(Button B: X\+(\d+), Y\+(\d+))");
      if (regex_match(line, match, buttonB_regex)) {
        machine.bx = stoll(match[1]);
        machine.by = stoll(match[2]);
      }

      getline(file, line);
      regex prize_regex(R"(Prize: X=(\d+), Y=(\d+))");
      if (regex_match(line, match, prize_regex)) {
        machine.px = stoll(match[1]);
        machine.py = stoll(match[2]);
      }

      machines.push_back(machine);
    }
  }

  file.close();

  cout << "Found " << machines.size() << " claw machines." << endl;
  long long totalTokens = 0;
  int prizesWon = 0;

  for (size_t i = 0; i < machines.size(); i++) {
    Machine &machine = machines[i];

    cout << "Machine " << (i + 1) << ":" << endl;
    cout << "  Button A: X+" << machine.ax << ", Y+" << machine.ay << endl;
    cout << "  Button B: X+" << machine.bx << ", Y+" << machine.by << endl;
    cout << "  Prize: X=" << machine.px << ", Y=" << machine.py << endl;
    ;

    pair<long long, long long> solution = solveMachine(machine);

    if (solution.first != -1 && solution.second != -1) {
      long long a = solution.first;
      long long b = solution.second;
      long long tokens = 3 * a + 1 * b;

      cout << "  Solution: A=" << a << ", B=" << b << " (Cost: " << tokens
           << " tokens)" << endl;

      long long checkX = a * machine.ax + b * machine.bx;
      long long checkY = a * machine.ay + b * machine.by;
      cout << "  Verification: X=" << checkX << " (target=" << machine.px
           << "), Y=" << checkY << " (target=" << machine.py << ")" << endl;

      totalTokens += tokens;
      prizesWon++;
    } else {
      cout << " No solution found." << endl;
    }
    cout << endl;
  }

  cout << "Results:" << endl;
  cout << "Prizes won: " << prizesWon << " out of " << machines.size() << endl;
  cout << "Total tokens spent: " << totalTokens << endl;

  return 0;
}
