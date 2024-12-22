#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Function to generate the next secret number
uint32_t nextSecret(uint32_t secret) {
    secret ^= (secret * 64);
    secret %= 16777216;
    secret ^= (secret / 32);
    secret %= 16777216;
    secret ^= (secret * 2048);
    secret %= 16777216;
    return secret;
}

// Function to calculate the best sequence
int findMostBananas(vector<uint32_t> &initialSecrets) {
    const int maxNumbers = 2000; // Number of secret numbers to generate per buyer
    vector<vector<int>> priceChanges;

    // Generate prices and their changes for each buyer
    for (uint32_t secret : initialSecrets) {
        vector<int> prices, changes;
        for (int i = 0; i < maxNumbers; ++i) {
            int price = secret % 10; // Extract the ones digit
            prices.push_back(price);
            secret = nextSecret(secret);
        }

        // Calculate price changes
        for (int i = 1; i < maxNumbers; ++i) {
            changes.push_back(prices[i] - prices[i - 1]);
        }
        priceChanges.push_back(changes);
    }

    // Find the best sequence of 4 changes
    int maxBananas = 0;
    vector<int> bestSequence;

    for (int a = -9; a <= 9; ++a) {
        for (int b = -9; b <= 9; ++b) {
            for (int c = -9; c <= 9; ++c) {
                for (int d = -9; d <= 9; ++d) {
                    vector<int> sequence = {a, b, c, d};
                    int totalBananas = 0;

                    // Check this sequence for each buyer
                    for (const auto &changes : priceChanges) {
                        for (int i = 0; i <= changes.size() - 4; ++i) {
                            if (changes[i] == a && changes[i + 1] == b &&
                                changes[i + 2] == c && changes[i + 3] == d) {
                                totalBananas += changes[i + 4] + 10; // Price when sequence occurs
                                break;
                            }
                        }
                    }

                    if (totalBananas > maxBananas) {
                        maxBananas = totalBananas;
                        bestSequence = sequence;
                    }
                }
            }
        }
    }

    // Output the best sequence (for debugging/verification)
    cout << "Best sequence: ";
    for (int x : bestSequence) cout << x << " ";
    cout << endl;

    return maxBananas;
}

int main() {
    ifstream inputFile("input.txt");
    vector<uint32_t> initialSecrets;

    // Read the input
    uint32_t secret;
    while (inputFile >> secret) {
        initialSecrets.push_back(secret);
    }

    // Find the most bananas
    int result = findMostBananas(initialSecrets);
    cout << "Most bananas: " << result << endl;

    return 0;
}


