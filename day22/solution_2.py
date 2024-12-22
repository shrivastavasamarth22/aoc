def next_secret(secret):
    """Generate the next secret number."""
    secret ^= (secret * 64)
    secret %= 16777216
    secret ^= (secret // 32)
    secret %= 16777216
    secret ^= (secret * 2048)
    secret %= 16777216
    return secret

def find_most_bananas(initial_secrets):
    """Find the best sequence of 4 changes to maximize bananas."""
    max_numbers = 2000  # Number of secret numbers to generate per buyer
    price_changes = []

    # Generate prices and their changes for each buyer
    for secret in initial_secrets:
        prices = []
        changes = []
        
        # Generate prices
        for _ in range(max_numbers):
            prices.append(secret % 10)  # Extract the ones digit
            secret = next_secret(secret)

        # Calculate price changes
        for i in range(1, max_numbers):
            changes.append(prices[i] - prices[i - 1])

        price_changes.append(changes)

    # Find the best sequence of 4 changes
    max_bananas = 0
    best_sequence = []

    for a in range(-9, 10):
        for b in range(-9, 10):
            for c in range(-9, 10):
                for d in range(-9, 10):
                    sequence = [a, b, c, d]
                    total_bananas = 0

                    # Check this sequence for each buyer
                    for changes in price_changes:
                        for i in range(len(changes) - 3):
                            if (changes[i] == a and changes[i + 1] == b and
                                changes[i + 2] == c and changes[i + 3] == d):
                                total_bananas += changes[i + 4] + 10  # Price when sequence occurs
                                break

                    if total_bananas > max_bananas:
                        max_bananas = total_bananas
                        best_sequence = sequence

    # Output the best sequence (for debugging/verification)
    print("Best sequence:", best_sequence)

    return max_bananas

if __name__ == "__main__":
    # Read initial secrets from input file
    with open("input.txt", "r") as file:
        initial_secrets = [int(line.strip()) for line in file]

    # Find the most bananas
    result = find_most_bananas(initial_secrets)
    print("Most bananas:", result)

