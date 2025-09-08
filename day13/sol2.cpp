#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

struct Machine {
  long long ax, ay, bx, by, px, py;
};

std::vector<Machine> parseMachines(const std::string &filename) {
  std::vector<Machine> machines;
  std::ifstream file(filename);
  std::string line;

  std::regex buttonRegex(R"(Button [AB]: X\+(\d+), Y\+(\d+))");
  std::regex prizeRegex(R"(Prize: X=(\d+), Y=(\d+))");
  std::smatch match;

  Machine current;
  int lineCount = 0;

  while (std::getline(file, line)) {
    if (line.empty())
      continue;

    if (lineCount % 3 == 0) { // Button A
      if (std::regex_match(line, match, buttonRegex)) {
        current.ax = std::stoll(match[1]);
        current.ay = std::stoll(match[2]);
      }
    } else if (lineCount % 3 == 1) { // Button B
      if (std::regex_match(line, match, buttonRegex)) {
        current.bx = std::stoll(match[1]);
        current.by = std::stoll(match[2]);
      }
    } else { // Prize
      if (std::regex_match(line, match, prizeRegex)) {
        current.px = std::stoll(match[1]) + 10000000000000LL; // Part 2 offset
        current.py = std::stoll(match[2]) + 10000000000000LL; // Part 2 offset
        machines.push_back(current);
      }
    }
    lineCount++;
  }

  return machines;
}

// Solve system of linear equations using Cramer's rule
// ax*a + bx*b = px
// ay*a + by*b = py
long long solveMachine(const Machine &m) {
  long long det = m.ax * m.by - m.ay * m.bx;

  if (det == 0) {
    // No unique solution
    return 0;
  }

  // Using Cramer's rule
  long long num_a = m.px * m.by - m.py * m.bx;
  long long num_b = m.ax * m.py - m.ay * m.px;

  // Check if solutions are integers
  if (num_a % det != 0 || num_b % det != 0) {
    return 0;
  }

  long long a = num_a / det;
  long long b = num_b / det;

  // Check if solutions are non-negative
  if (a < 0 || b < 0) {
    return 0;
  }

  // Verify the solution
  if (a * m.ax + b * m.bx != m.px || a * m.ay + b * m.by != m.py) {
    return 0;
  }

  return 3 * a + b;
}

int main() {
  std::vector<Machine> machines = parseMachines("input.txt");

  long long totalTokens = 0;
  int prizesWon = 0;

  for (const auto &machine : machines) {
    long long tokens = solveMachine(machine);
    if (tokens > 0) {
      totalTokens += tokens;
      prizesWon++;
    }
  }

  std::cout << "Prizes won: " << prizesWon << std::endl;
  std::cout << "Total tokens needed: " << totalTokens << std::endl;

  return 0;
}
