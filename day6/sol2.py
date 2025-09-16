def simulate_patrol_with_loop_detection(grid, start_pos, start_dir):
    """Simulate guard patrol and detect if guard gets stuck in a loop"""
    rows, cols = len(grid), len(grid[0])

    def turn_right(direction):
        dr, dc = direction
        return (dc, -dr)

    # Track (position, direction) states to detect loops
    visited_states = set()
    current_pos = start_pos
    current_dir = start_dir

    while True:
        # Check if we've been in this state before (loop detection)
        state = (current_pos, current_dir)
        if state in visited_states:
            return True  # Loop detected
        visited_states.add(state)

        # Calculate next position
        r, c = current_pos
        dr, dc = current_dir
        next_r, next_c = r + dr, c + dc

        # Check if next position is out of bounds
        if next_r < 0 or next_r >= rows or next_c < 0 or next_c >= cols:
            return False  # Guard leaves the area - no loop

        # Check if there's an obstacle ahead
        if grid[next_r][next_c] == "#":
            # Turn right 90 degrees
            current_dir = turn_right(current_dir)
        else:
            # Move forward
            current_pos = (next_r, next_c)


def get_original_path(grid, start_pos, start_dir):
    """Get the guard's original patrol path without any additional obstacles"""
    rows, cols = len(grid), len(grid[0])

    def turn_right(direction):
        dr, dc = direction
        return (dc, -dr)

    visited = set()
    current_pos = start_pos
    current_dir = start_dir
    visited.add(current_pos)

    while True:
        # Calculate next position
        r, c = current_pos
        dr, dc = current_dir
        next_r, next_c = r + dr, c + dc

        # Check if next position is out of bounds
        if next_r < 0 or next_r >= rows or next_c < 0 or next_c >= cols:
            break

        # Check if there's an obstacle ahead
        if grid[next_r][next_c] == "#":
            # Turn right 90 degrees
            current_dir = turn_right(current_dir)
        else:
            # Move forward
            current_pos = (next_r, next_c)
            visited.add(current_pos)

    return visited


def solve_part2():
    """Find all positions where placing an obstruction would create a loop"""
    # Read the input file
    with open("input.txt", "r") as f:
        grid = [list(line.strip()) for line in f.readlines()]

    rows, cols = len(grid), len(grid[0])

    # Find the guard's starting position and direction
    guard_pos = None
    guard_dir = None

    # Direction mappings: up, right, down, left
    directions = {
        "^": (-1, 0),  # up
        ">": (0, 1),  # right
        "v": (1, 0),  # down
        "<": (0, -1),  # left
    }

    # Find guard starting position
    for r in range(rows):
        for c in range(cols):
            if grid[r][c] in directions:
                guard_pos = (r, c)
                guard_dir = directions[grid[r][c]]
                grid[r][c] = "."  # Replace guard symbol with empty space
                break
        if guard_pos:
            break

    # Get the original path to optimize our search
    # Only test positions where the guard originally walks
    original_path = get_original_path(grid, guard_pos, guard_dir)

    valid_obstruction_positions = 0
    tested_positions = 0

    # Test each position on the original path (except starting position)
    for r, c in original_path:
        # Skip the starting position (guard is there and would notice)
        if (r, c) == guard_pos:
            continue

        # Skip if there's already an obstacle
        if grid[r][c] == "#":
            continue

        tested_positions += 1

        # Try placing an obstacle at this position
        grid[r][c] = "#"

        # Test if this creates a loop
        creates_loop = simulate_patrol_with_loop_detection(grid, guard_pos, guard_dir)

        if creates_loop:
            valid_obstruction_positions += 1

        # Remove the obstacle (restore original state)
        grid[r][c] = "."

    print(f"Tested {tested_positions} positions from the original path")
    print(f"Found {valid_obstruction_positions} positions that would create a loop")

    return valid_obstruction_positions


# Solve Part 2
if __name__ == "__main__":
    result = solve_part2()
    print(f"\nAnswer: {result} different positions could be chosen for the obstruction")
