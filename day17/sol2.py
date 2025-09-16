def parse_input(filename):
    """Parse the input file to get initial register values and program"""
    with open(filename, 'r') as f:
        lines = [line.strip() for line in f.readlines()]
    
    reg_a = int(lines[0].split(': ')[1])
    reg_b = int(lines[1].split(': ')[1])
    reg_c = int(lines[2].split(': ')[1])
    
    program_str = lines[4].split(': ')[1]
    program = [int(x) for x in program_str.split(',')]
    
    return reg_a, reg_b, reg_c, program

def get_combo_value(operand, reg_a, reg_b, reg_c):
    """Get the value of a combo operand"""
    if operand <= 3:
        return operand
    elif operand == 4:
        return reg_a
    elif operand == 5:
        return reg_b
    elif operand == 6:
        return reg_c
    else:
        raise ValueError(f"Invalid combo operand: {operand}")

def run_program(reg_a, reg_b, reg_c, program):
    """Run the chronospatial computer program and return output"""
    ip = 0  # instruction pointer
    output = []
    
    while ip < len(program):
        if ip + 1 >= len(program):
            break
            
        opcode = program[ip]
        operand = program[ip + 1]
        
        if opcode == 0:  # adv - A division
            denominator = 2 ** get_combo_value(operand, reg_a, reg_b, reg_c)
            reg_a = reg_a // denominator
            
        elif opcode == 1:  # bxl - B XOR literal
            reg_b = reg_b ^ operand
            
        elif opcode == 2:  # bst - combo mod 8 to B
            reg_b = get_combo_value(operand, reg_a, reg_b, reg_c) % 8
            
        elif opcode == 3:  # jnz - jump if A not zero
            if reg_a != 0:
                ip = operand
                continue
                
        elif opcode == 4:  # bxc - B XOR C
            reg_b = reg_b ^ reg_c
            
        elif opcode == 5:  # out - output combo mod 8
            output.append(get_combo_value(operand, reg_a, reg_b, reg_c) % 8)
            
        elif opcode == 6:  # bdv - A division to B
            denominator = 2 ** get_combo_value(operand, reg_a, reg_b, reg_c)
            reg_b = reg_a // denominator
            
        elif opcode == 7:  # cdv - A division to C
            denominator = 2 ** get_combo_value(operand, reg_a, reg_b, reg_c)
            reg_c = reg_a // denominator
            
        ip += 2
    
    return output

def find_self_replicating_a(program, initial_b=0, initial_c=0):
    """
    Find the lowest positive value for register A that makes the program output itself.
    
    Strategy: Work backwards from the desired output (the program itself).
    Since many programs divide A by powers of 2, we can often work backwards
    by trying different values and seeing which ones produce the right output.
    """
    target = program.copy()
    
    # For efficiency, we'll use a recursive/iterative approach
    # Starting from small values and working up
    
    # First, let's try a direct search for smaller programs
    if len(program) <= 10:  # For small programs, brute force is feasible
        for a in range(1, 1000000):  # Try up to 1 million
            output = run_program(a, initial_b, initial_c, program)
            if output == target:
                return a
                
        # If not found in reasonable range, try larger values
        for a in range(1000000, 10000000, 1000):  # Try every 1000th value
            output = run_program(a, initial_b, initial_c, program)
            if output == target:
                # Found a candidate, now search nearby for the actual minimum
                for candidate in range(max(1, a - 1000), a + 1000):
                    candidate_output = run_program(candidate, initial_b, initial_c, program)
                    if candidate_output == target:
                        return candidate
    
    # For longer programs, we need a smarter approach
    # Analyze the program structure to understand how A affects output
    return find_self_replicating_smart(program, initial_b, initial_c)

def find_self_replicating_smart(program, initial_b=0, initial_c=0):
    """
    Smart approach for finding self-replicating A value.
    
    Many programs that output themselves have a structure where:
    1. They output digits by taking A modulo 8 (or similar)
    2. They divide A by 8 (or similar power of 2) in each iteration
    3. They loop until A becomes 0
    
    This means we can work backwards: if we know what the last few outputs
    should be, we can determine what A should be before the last few divisions.
    """
    target = program.copy()
    
    # Try to find a pattern by analyzing a few runs
    # Look for programs that divide A by 8 repeatedly (common pattern)
    candidates = []
    
    # Work backwards from the target output
    # If the program outputs by taking A % 8, then divides A by 8,
    # we can reconstruct possible A values
    
    def backtrack(remaining_output, current_a):
        if not remaining_output:
            # We've matched all output, test this A value
            test_output = run_program(current_a, initial_b, initial_c, program)
            if test_output == target:
                return current_a
            return None
        
        # Try different values that could produce the last output digit
        last_digit = remaining_output[-1]
        remaining = remaining_output[:-1]
        
        # If the program outputs (A % 8) then does A //= 8,
        # then current_a * 8 + last_digit could be the previous A
        for offset in range(8):  # Try all possible remainders
            next_a = current_a * 8 + offset
            if next_a == 0:
                continue
                
            # Quick test: run one iteration and see if it outputs the right digit
            test_output = run_program(next_a, initial_b, initial_c, program)
            if test_output and test_output[0] == last_digit:
                result = backtrack(remaining, next_a)
                if result is not None:
                    return result
        
        return None
    
    # Start backtracking from A = 0
    result = backtrack(target, 0)
    if result is not None:
        return result
    
    # If backtracking doesn't work, fall back to brute force
    print("Smart approach failed, falling back to brute force...")
    for a in range(1, 1000000):
        output = run_program(a, initial_b, initial_c, program)
        if output == target:
            return a
    
    return None

def solve_part2():
    """Solve part 2: find the lowest positive A value for self-replication"""
    # Parse input
    initial_a, initial_b, initial_c, program = parse_input('input.txt')
    
    print(f"Original register values: A={initial_a}, B={initial_b}, C={initial_c}")
    print(f"Program: {','.join(map(str, program))}")
    print(f"Target output (program itself): {','.join(map(str, program))}")
    print()
    
    # Find the self-replicating A value
    print("Searching for self-replicating A value...")
    result_a = find_self_replicating_a(program, initial_b, initial_c)
    
    if result_a is not None:
        print(f"Found self-replicating A value: {result_a}")
        
        # Verify the result
        output = run_program(result_a, initial_b, initial_c, program)
        print(f"Verification - Output: {','.join(map(str, output))}")
        print(f"Verification - Target:  {','.join(map(str, program))}")
        print(f"Verification - Match: {output == program}")
        
        return result_a
    else:
        print("Could not find self-replicating A value in reasonable range")
        return None

def test_example():
    """Test with the example from the problem"""
    print("=== Testing Example ===")
    program = [0, 3, 5, 4, 3, 0]
    print(f"Example program: {','.join(map(str, program))}")
    
    # The problem states that A=117440 should work
    test_a = 117440
    output = run_program(test_a, 0, 0, program)
    print(f"A={test_a} produces output: {','.join(map(str, output))}")
    print(f"Expected: {','.join(map(str, program))}")
    print(f"Match: {output == program}")
    
    # Now find the minimum A
    result_a = find_self_replicating_a(program, 0, 0)
    print(f"Minimum A found: {result_a}")
    print()

if __name__ == "__main__":
    # Test with example first
    test_example()
    
    # Solve the actual problem
    answer = solve_part2()
    if answer is not None:
        print(f"\nAnswer: The lowest positive initial value for register A is {answer}")
