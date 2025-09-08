#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

struct Robot {
  int x, y;   // position
  int vx, vy; // velocity
};

vector<Robot> parseInput(const string &filename) {
  vector<Robot> robots;
  ifstream file(filename);
  string line;

  // Regular expression to parse: p=x,y v=vx,vy
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

// Handle wrapping with proper modulo for negative numbers
int wrap(int pos, int velocity, int time, int size) {
  long long newPos = (long long)pos + (long long)velocity * time;
  newPos = ((newPos % size) + size) % size; // Handle negative modulo correctly
  return (int)newPos;
}

int calculateSafetyFactor(const vector<Robot> &robots, int width, int height,
                          int time) {
  int midX = width / 2;  // 50 for width 101
  int midY = height / 2; // 51 for height 103

  int quadrants[4] = {0, 0, 0,
                      0}; // top-left, top-right, bottom-left, bottom-right

  for (const Robot &robot : robots) {
    // Calculate position after time seconds
    int finalX = wrap(robot.x, robot.vx, time, width);
    int finalY = wrap(robot.y, robot.vy, time, height);

    // Skip robots exactly in the middle (horizontally or vertically)
    if (finalX == midX || finalY == midY) {
      continue;
    }

    // Determine quadrant
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

  // Calculate safety factor
  return quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3];
}

void printGrid(const vector<Robot> &robots, int width, int height, int time) {
  vector<vector<int>> grid(height, vector<int>(width, 0));

  for (const Robot &robot : robots) {
    int finalX = wrap(robot.x, robot.vx, time, width);
    int finalY = wrap(robot.y, robot.vy, time, height);
    grid[finalY][finalX]++;
  }

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (grid[y][x] == 0) {
        cout << '.';
      } else {
        cout << '#';
      }
    }
    cout << endl;
  }
  cout << endl;
}

int countUniquePositions(const vector<Robot> &robots, int width, int height,
                         int time) {
  vector<vector<bool>> grid(height, vector<bool>(width, false));
  int uniqueCount = 0;

  for (const Robot &robot : robots) {
    int finalX = wrap(robot.x, robot.vx, time, width);
    int finalY = wrap(robot.y, robot.vy, time, height);
    if (!grid[finalY][finalX]) {
      grid[finalY][finalX] = true;
      uniqueCount++;
    }
  }

  return uniqueCount;
}

// Check if robots form a connected pattern (might indicate a tree)
bool hasLargeConnectedComponent(const vector<Robot> &robots, int width,
                                int height, int time) {
  vector<vector<bool>> grid(height, vector<bool>(width, false));

  // Mark robot positions
  for (const Robot &robot : robots) {
    int finalX = wrap(robot.x, robot.vx, time, width);
    int finalY = wrap(robot.y, robot.vy, time, height);
    grid[finalY][finalX] = true;
  }

  // Find largest connected component using DFS
  vector<vector<bool>> visited(height, vector<bool>(width, false));
  int maxComponent = 0;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (grid[y][x] && !visited[y][x]) {
        // BFS/DFS to find component size
        int componentSize = 0;
        vector<pair<int, int>> stack = {{y, x}};

        while (!stack.empty()) {
          auto [cy, cx] = stack.back();
          stack.pop_back();

          if (cy < 0 || cy >= height || cx < 0 || cx >= width ||
              visited[cy][cx] || !grid[cy][cx]) {
            continue;
          }

          visited[cy][cx] = true;
          componentSize++;

          // Add 8-connected neighbors
          for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
              if (dy != 0 || dx != 0) {
                stack.push_back({cy + dy, cx + dx});
              }
            }
          }
        }

        maxComponent = max(maxComponent, componentSize);
      }
    }
  }

  // If a large portion of robots form a connected component, might be the tree
  return maxComponent > robots.size() / 3;
}

int main() {
  const int WIDTH = 101;
  const int HEIGHT = 103;

  vector<Robot> robots = parseInput("input.txt");

  cout << "Loaded " << robots.size() << " robots" << endl;

  // Part 1
  int safetyFactor = calculateSafetyFactor(robots, WIDTH, HEIGHT, 100);
  cout << "Part 1 - Safety factor after 100 seconds: " << safetyFactor << endl;

  // Part 2 - Find the Easter egg
  cout << "\nSearching for Christmas tree pattern..." << endl;

  int minSafetyFactor = INT_MAX;
  int minSafetyTime = 0;
  int maxUniquePositions = 0;
  int maxUniqueTime = 0;

  for (int t = 1; t <= 10000; t++) { // Check first 10000 seconds
    int sf = calculateSafetyFactor(robots, WIDTH, HEIGHT, t);
    int uniquePos = countUniquePositions(robots, WIDTH, HEIGHT, t);

    if (sf < minSafetyFactor) {
      minSafetyFactor = sf;
      minSafetyTime = t;
    }

    if (uniquePos > maxUniquePositions) {
      maxUniquePositions = uniquePos;
      maxUniqueTime = t;
    }

    // Check for large connected components (potential tree)
    if (hasLargeConnectedComponent(robots, WIDTH, HEIGHT, t)) {
      cout << "\nPotential Christmas tree at time " << t << ":" << endl;
      cout << "Safety factor: " << sf << ", Unique positions: " << uniquePos
           << endl;
      printGrid(robots, WIDTH, HEIGHT, t);

      // This is likely our answer
      cout << "Part 2 - Christmas tree appears at: " << t << " seconds" << endl;
      return 0;
    }

    if (t % 1000 == 0) {
      cout << "Checked " << t << " seconds..." << endl;
    }
  }

  cout << "\nMinimum safety factor: " << minSafetyFactor << " at time "
       << minSafetyTime << endl;
  cout << "Maximum unique positions: " << maxUniquePositions << " at time "
       << maxUniqueTime << endl;

  // Show the configuration with minimum safety factor
  cout << "\nConfiguration at minimum safety factor (time " << minSafetyTime
       << "):" << endl;
  printGrid(robots, WIDTH, HEIGHT, minSafetyTime);

  cout << "Part 2 - Best guess: " << minSafetyTime << " seconds" << endl;

  return 0;
}
