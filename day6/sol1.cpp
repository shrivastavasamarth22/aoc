#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int row;
  int col;
  char dir;
} GuardState;

typedef struct {
  char **data;
  int rows;
  int cols;
} Map;

typedef struct {
  GuardState *states;
  int count;
  int capacity;
} VisitedStates;

void turn_guard(char *guard_dir);
void turn_guard(char *guard_dir);
void get_next_pos(int dest[2], int current_pos[2], char guard_dir);
void add_visited_state(VisitedStates *visited, GuardState state);
int has_visited_state(const VisitedStates *visited, GuardState state);
void free_map(Map *map);
void free_visited_states(VisitedStates *visited);

int main() {
  FILE *file_ptr = fopen("input.txt", "r");
  if (file_ptr == NULL) {
    printf("Error: File can't be opened.\n");
    return EXIT_FAILURE;
  }

  Map map = {.data = NULL, .rows = 0, .cols = 0};
  char buffer[1024]; // Assume lines are not longer than 1023 chars

  // First pass: determine map dimensions
  if (fgets(buffer, sizeof(buffer), file_ptr) != NULL) {
    map.cols = strlen(buffer);
    if (buffer[map.cols - 1] == '\n') {
      buffer[map.cols - 1] = '\0';
      map.cols--;
    }
    map.rows = 1;
    while (fgets(buffer, sizeof(buffer), file_ptr) != NULL) {
      map.rows++;
    }
  }
  rewind(file_ptr); // Go back to the start of the file

  // Allocate memory for the map
  map.data = (char **)malloc(map.rows * sizeof(char *));
  for (int i = 0; i < map.rows; i++) {
    map.data[i] = (char *)malloc((map.cols + 1) * sizeof(char));
    fgets(map.data[i], map.cols + 2, file_ptr); // Read line including newline
    map.data[i][map.cols] = '\0';               // Ensure null termination
  }
  fclose(file_ptr);

  // Find guard's starting position and direction
  int guard_pos[2] = {-1, -1};
  char guard_dir = '\0';
  for (int r = 0; r < map.rows; r++) {
    for (int c = 0; c < map.cols; c++) {
      char cell = map.data[r][c];
      if (cell == '^' || cell == '>' || cell == 'v' || cell == '<') {
        guard_pos[0] = r;
        guard_pos[1] = c;
        guard_dir = cell;
        map.data[r][c] = '.'; // Clear the guard symbol
        break;
      }
    }
    if (guard_pos[0] != -1)
      break;
  }

  if (guard_pos[0] == -1) {
    printf("Error: Guard not found on map.\n");
    free_map(&map);
    return EXIT_FAILURE;
  }

  // Use a separate 2D array to track visited positions for the final count
  int **visited_pos = (int **)malloc(map.rows * sizeof(int *));
  for (int i = 0; i < map.rows; i++) {
    visited_pos[i] = (int *)calloc(map.cols, sizeof(int));
  }

  // Initialize the dynamic array for visited states (for loop detection)
  VisitedStates visited_states = {
      .states = malloc(100 * sizeof(GuardState)), .count = 0, .capacity = 100};

  int distinct_pos_count = 0;

  // --- Main Simulation Loop ---
  while (1) {
    // Mark current position as visited if it's the first time
    if (visited_pos[guard_pos[0]][guard_pos[1]] == 0) {
      visited_pos[guard_pos[0]][guard_pos[1]] = 1;
      distinct_pos_count++;
    }

    // --- Loop Detection ---
    GuardState current_state = {guard_pos[0], guard_pos[1], guard_dir};
    if (has_visited_state(&visited_states, current_state)) {
      break; // An infinite loop is detected, so we stop.
    }
    add_visited_state(&visited_states, current_state);

    // Determine next position
    int next_pos[2];
    get_next_pos(next_pos, guard_pos, guard_dir);

    // Check if the guard leaves the map
    if (next_pos[0] < 0 || next_pos[0] >= map.rows || next_pos[1] < 0 ||
        next_pos[1] >= map.cols) {
      break; // Guard walked off the map.
    }

    // Check for obstacles
    if (map.data[next_pos[0]][next_pos[1]] == '#') {
      turn_guard(&guard_dir); // Turn right
    } else {
      // Move forward
      guard_pos[0] = next_pos[0];
      guard_pos[1] = next_pos[1];
    }
  }

  printf("Distinct positions visited: %d\n", distinct_pos_count);

  // Free all allocated memory
  for (int i = 0; i < map.rows; i++) {
    free(visited_pos[i]);
  }
  free(visited_pos);
  free_map(&map);
  free_visited_states(&visited_states);

  return 0;
}

// --- Helper Function Implementations ---

void turn_guard(char *guard_dir) {
  switch (*guard_dir) {
  case '^':
    *guard_dir = '>';
    break;
  case '>':
    *guard_dir = 'v';
    break;
  case 'v':
    *guard_dir = '<';
    break;
  case '<':
    *guard_dir = '^';
    break;
  }
}

void get_next_pos(int dest[2], int current_pos[2], char guard_dir) {
  dest[0] = current_pos[0];
  dest[1] = current_pos[1];
  switch (guard_dir) {
  case '^':
    dest[0]--;
    break;
  case '>':
    dest[1]++;
    break;
  case 'v':
    dest[0]++;
    break;
  case '<':
    dest[1]--;
    break;
  }
}

void add_visited_state(VisitedStates *visited, GuardState state) {
  if (visited->count >= visited->capacity) {
    visited->capacity *= 2;
    visited->states = (GuardState *)realloc(
        visited->states, visited->capacity * sizeof(GuardState));
  }
  visited->states[visited->count++] = state;
}

int has_visited_state(const VisitedStates *visited, GuardState state) {
  for (int i = 0; i < visited->count; i++) {
    if (visited->states[i].row == state.row &&
        visited->states[i].col == state.col &&
        visited->states[i].dir == state.dir) {
      return 1; // Found a matching state
    }
  }
  return 0; // No match found
}

void free_map(Map *map) {
  if (map->data) {
    for (int i = 0; i < map->rows; i++) {
      free(map->data[i]);
    }
    free(map->data);
  }
}

void free_visited_states(VisitedStates *visited) {
  if (visited->states) {
    free(visited->states);
  }
}
