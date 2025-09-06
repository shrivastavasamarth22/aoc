#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct File {
  int id;
  int start;
  int size;
};

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
  vector<File> files;
  int fileId = 0;
  bool isFile = true; // Start with a file
  int position = 0;

  for (char c : diskMap) {
    int length = c - '0';

    if (isFile) {
      // Add file blocks and record file info
      files.push_back({fileId, position, length});
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

    position += length;
    isFile = !isFile; // Alternate between file and free space
  }

  cout << "Initial disk layout (" << disk.size() << " blocks):" << endl;
  for (size_t i = 0; i < min(disk.size(), size_t(100)); i++) {
    if (disk[i] == -1) {
      cout << '.';
    } else {
      cout << disk[i] % 10; // Show only last digit for readability
    }
  }
  if (disk.size() > 100)
    cout << "...";
  cout << endl << endl;

  // Sort files by ID in descending order (highest ID first)
  sort(files.begin(), files.end(),
       [](const File &a, const File &b) { return a.id > b.id; });

  // Attempt to move each file exactly once, in order of decreasing file ID
  for (const File &currentFile : files) {
    cout << "Attempting to move file " << currentFile.id << " (size "
         << currentFile.size << ")" << endl;

    // Find the current position of this file in the disk
    int currentStart = -1;
    for (size_t i = 0; i < disk.size(); i++) {
      if (disk[i] == currentFile.id) {
        currentStart = i;
        break;
      }
    }

    if (currentStart == -1)
      continue; // File not found (shouldn't happen)

    // Look for a contiguous span of free space to the left that can fit this
    // file
    bool moved = false;
    for (size_t start = 0; start <= (size_t)currentStart - currentFile.size;
         start++) {
      // Check if we have enough contiguous free space starting at 'start'
      bool canFit = true;
      for (int offset = 0; offset < currentFile.size; offset++) {
        if (start + offset >= disk.size() || disk[start + offset] != -1) {
          canFit = false;
          break;
        }
      }

      if (canFit) {
        // Move the file
        cout << "  Moving file " << currentFile.id << " from position "
             << currentStart << " to position " << start << endl;

        // Clear the old position
        for (int offset = 0; offset < currentFile.size; offset++) {
          disk[currentStart + offset] = -1;
        }

        // Set the new position
        for (int offset = 0; offset < currentFile.size; offset++) {
          disk[start + offset] = currentFile.id;
        }

        moved = true;
        break; // Found the leftmost suitable position
      }
    }

    if (!moved) {
      cout << "  File " << currentFile.id << " could not be moved" << endl;
    }
  }

  cout << "\nCompacted disk layout:" << endl;
  for (size_t i = 0; i < min(disk.size(), size_t(100)); i++) {
    if (disk[i] == -1) {
      cout << '.';
    } else {
      cout << disk[i] % 10; // Show only last digit for readability
    }
  }
  if (disk.size() > 100)
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
