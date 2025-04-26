#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void wordleHelper(
    std::string current,
    std::string floating,
    const std::set<std::string>& dict,
    std::set<std::string>& results,
    size_t index); // Helper function for wordle()

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> results; // Set holds valid words from wordleHelper()
    wordleHelper(in, floating, dict, results, 0); // Call helper function
    return results;
}

// Define any helper functions here
// Helper recursive function
void wordleHelper(
    std::string current,
    std::string floating,
    const std::set<std::string>& dict,
    std::set<std::string>& results,
    size_t index)
{
    if (index == current.size()) { // If filled out all characters in string, check if word is valid
        // Base case: if the string is full
        if (floating.empty() && dict.find(current) != dict.end()) { // If current word is valid, add to results set
            results.insert(current);
        }
        return;
    }
    // If current character is already set, recursively call wordleHelper() to move to next character
    if (current[index] != '-') {
        wordleHelper(current, floating, dict, results, index + 1);
    }
    else { // If it's a blank '-', meaning need to set character at current index
        if (!floating.empty()) { // Try all floating letters first
            for (size_t i = 0; i < floating.size(); ++i) {
                char c = floating[i];
                current[index] = c;
                std::string newFloating = floating;
                newFloating.erase(i, 1); // Remove used floating character from floating string
                wordleHelper(current, newFloating, dict, results, index + 1); // Recursively call wordleHelper() with updated floating string
            }
        }
        // If there are more blanks than floating letters left, can try any letter
        if (current.size() - index > floating.size()) {
            for (char c = 'a'; c <= 'z'; ++c) {
                current[index] = c;
                wordleHelper(current, floating, dict, results, index + 1); // Recursively call wordleHelper() to test every character
            }
        }
    }
}
