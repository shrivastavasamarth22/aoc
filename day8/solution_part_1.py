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

def navigate_network(instructions, network):
    """
    Navigate the network from AAA to ZZZ.

    Returns:
        - Number of steps to reach ZZZ.
    """

    current_node = 'AAA'
    steps = 0
    instruction_length = len(instructions)

    while current_node != 'ZZZ':
        instruction = instructions[steps % instruction_length]

        if instruction == 'L':
            current_node = network[current_node][0]
        else:
            current_node = network[current_node][1]

        steps += 1

    return steps

def main():
    instructions, network = parse_input('input.txt')
    total_steps = navigate_network(instructions, network)
    print("Total steps: ", total_steps)

if __name__ == '__main__':
    main()
