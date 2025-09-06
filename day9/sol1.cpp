#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
  ifstream file("input.txt");
  if (!file.is_open()) {
    cerr << "Error: Could not open input.txt" << endl;
    return 1;
  }

  string diskMap;
  getline(file, diskMap);
  file.close();

  // Parse the disk map to create the initial layout
  vector<int>
      disk; // -1 represents free space, non-negative numbers are file IDs
  int fileId = 0;
  bool isFile = true; // Start with a file

  for (char c : diskMap) {
    int length = c - '0';

    if (isFile) {
      // Add file blocks
      for (int i = 0; i < length; i++) {
        disk.push_back(fileId);
      }
      fileId++;
    } else {
      // Add free space blocks
      for (int i = 0; i < length; i++) {
        disk.push_back(-1);
      }
    }

    isFile = !isFile; // Alternate between file and free space
  }

  cout << "Initial disk layout (" << disk.size() << " blocks):" << endl;
  for (size_t i = 0; i < min(disk.size(), size_t(80)); i++) {
    if (disk[i] == -1) {
      cout << '.';
    } else {
      cout << disk[i] % 10; // Show only last digit for readability
    }
  }
  if (disk.size() > 80)
    cout << "...";
  cout << endl << endl;

  // Compact the disk by moving file blocks from the end to leftmost free space
  size_t leftPtr = 0;
  size_t rightPtr = disk.size() - 1;

  while (leftPtr < rightPtr) {
    // Find the next free space from the left
    while (leftPtr < disk.size() && disk[leftPtr] != -1) {
      leftPtr++;
    }

    // Find the next file block from the right
    while (rightPtr > 0 && disk[rightPtr] == -1) {
      rightPtr--;
    }

    // If we found both a free space and a file block, swap them
    if (leftPtr < rightPtr && leftPtr < disk.size() && rightPtr < disk.size()) {
      disk[leftPtr] = disk[rightPtr];
      disk[rightPtr] = -1;
      leftPtr++;
      rightPtr--;
    } else {
      break;
    }
  }

  cout << "Compacted disk layout:" << endl;
  for (size_t i = 0; i < min(disk.size(), size_t(80)); i++) {
    if (disk[i] == -1) {
      cout << '.';
    } else {
      cout << disk[i] % 10; // Show only last digit for readability
    }
  }
  if (disk.size() > 80)
    cout << "...";
  cout << endl << endl;

  // Calculate the filesystem checksum
  long long checksum = 0;
  for (size_t pos = 0; pos < disk.size(); pos++) {
    if (disk[pos] != -1) { // Skip free space
      checksum += pos * disk[pos];
    }
  }

  cout << "Filesystem checksum: " << checksum << endl;

  return 0;
}
