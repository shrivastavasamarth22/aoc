def solve_guard_patrol():
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

    # Direction rotation (turn right 90 degrees)
    def turn_right(direction):
        dr, dc = direction
        # (up -> right -> down -> left -> up)
        # (-1,0) -> (0,1) -> (1,0) -> (0,-1) -> (-1,0)
        return (dc, -dr)

    visited = set()
    visited.add(guard_pos)

    current_pos = guard_pos
    current_dir = guard_dir

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

    return len(visited)


# Solve the problem
result = solve_guard_patrol()
print(
    f"The guard will visit {result} distinct positions before leaving the mapped area."
)
