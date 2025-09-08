#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

struct Robot {
  int x, y;
  int vx, vy;
};

vector<Robot> parseInput(const string &filename) {
  vector<Robot> robots;
  ifstream file(filename);
  string line;

  regex pattern(R"(p=(-?\d+),(-?\d+) v=(-?\d+),(-?\d+))");

  smatch match;

  while (getline(file, line)) {
    if (regex_match(line, match, pattern)) {
      Robot robot;
      robot.x = stoi(match[1]);
      robot.y = stoi(match[2]);
      robot.vx = stoi(match[3]);
      robot.vy = stoi(match[4]);
      robots.push_back(robot);
    }
  }
  return robots;
}

int wrap(int pos, int velocity, int time, int size) {
  long long newPos = (long long)pos + (long long)velocity * time;
  newPos = ((newPos % size) + size) % size;
  return (int)newPos;
}

int calculateSafetyFactor(const vector<Robot> &robots, int width, int height,
                          int time) {
  int midX = width / 2;
  int midY = height / 2;

  int quadrants[4] = {0, 0, 0, 0};

  for (const Robot &robot : robots) {
    int finalX = wrap(robot.x, robot.vx, time, width);
    int finalY = wrap(robot.y, robot.vy, time, height);

    if (finalX == midX || finalY == midY) {
      continue;
    }

    if (finalX < midX && finalY < midY) {
      quadrants[0]++; // top-left
    } else if (finalX > midX && finalY < midY) {
      quadrants[1]++; // top-right
    } else if (finalX < midX && finalY > midY) {
      quadrants[2]++; // bottom-left
    } else if (finalX > midX && finalY > midY) {
      quadrants[3]++; // bottom-right
    }
  }

  int safetyFactor = quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3];

  cout << "Quadrant counts: " << quadrants[0] << ", " << quadrants[1] << ", "
       << quadrants[2] << ", " << quadrants[3] << endl;

  return safetyFactor;
}

int main() {
  const int WIDTH = 101;
  const int HEIGHT = 103;
  const int TIME = 100;

  vector<Robot> robots = parseInput("input.txt");

  cout << "Loaded " << robots.size() << " robots." << endl;

  int safetyFactor = calculateSafetyFactor(robots, WIDTH, HEIGHT, TIME);

  cout << "Safety Factor after " << TIME << " seconds: " << safetyFactor
       << endl;

  return 0;
}
