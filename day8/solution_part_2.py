import math

def parse_input(filename):
    """
    Parse the input file into instructions and a network dictionary.

    Returns:
        - instructions (str): Sequence of left/right instructions.
        - network (dict): Mapping of nodes to their left and right neighbors.
    """

    with open(filename, 'r') as file:
        instructions = file.readline().strip()

        file.readline()

        network = {}
        for line in file:
            node, connections = line.strip().split(' = ')

            left, right = connections.strip('()').split(', ')
            network[node] = (left, right)

    return instructions, network

def find_cycle_length(start_node, instructions, network):
    """
    Find the cycle length for a single starting node.

    Returns:
        - Number of steps to first node ending with Z.
    """

    current_node = start_node
    steps = 0
    instruction_length = len(instructions)

    while not current_node.endswith('Z'):
        instruction = instructions[steps % instruction_length]

        if instruction == 'L':
            current_node = network[current_node][0]
        else:
            current_node = network[current_node][1]

        steps += 1

    return steps

def navigate_network(instructions, network):
    """
    Navigate the network starting from all nodes ending with A.

    Returns:
        - Least Common Multiple of all cycle lengths
    """

    start_nodes = [node for node in network.keys() if node.endswith('A')]
    cycle_lengths = [find_cycle_length(node, instructions, network) for node in start_nodes]

    return math.lcm(*cycle_lengths)

def main():
    instructions, network = parse_input('input.txt')
    total_steps = navigate_network(instructions, network)
    print("Total steps: ", total_steps)

if __name__ == '__main__':
    main()
