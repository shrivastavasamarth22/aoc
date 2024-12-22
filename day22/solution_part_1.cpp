#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Function to mix a value into the secret number using XOR
long long mix(long long secret, long long value) {
	return secret ^ value;
}

// Function to prune the secret number using modulo
long long prune(long long secret) {
	return secret % 16777216;
}

// Function to generate the next secret number
long long generateNextSecret(long long secret) {
	// Step 1: Multiply by 64
	long long result = mix(secret, secret * 64);
	result = prune(result);

	// Step 2: Divide by 32
	result = mix(result, result / 32);
	result = prune(result);

	// Step 3: Multiply by 2048
	result = mix(result, result * 2048);
	result = prune(result);

	return result;
}

int main() {
	ifstream inFile("input.txt");
	vector<long long> initialSecrets;
	long long secret;

	// Read initial secrets from file
	while (inFile >> secret) {
		initialSecrets.push_back(secret);
	}

	long long sum = 0;

	// Process each buyer's secret sequence
	for (long long initialSecret : initialSecrets) {
		long long currentSecret = initialSecret;

		// Generate 2000 new secret numbers
		for (int i = 0; i < 2000; i++) {
			currentSecret = generateNextSecret(currentSecret);
		}

		// Add the 2000th number to the sum
		sum += currentSecret;
	}

	cout << "Sum of 2000th secret numbers: " << sum << endl;

	return 0;
}
