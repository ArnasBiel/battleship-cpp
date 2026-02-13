#include "Extras.h"
#include <cctype>      // For toupper
#include <stdexcept>   // For std::invalid_argument
#include <string>      // For std::string
#include <iostream>    // If needed for debugging purposes
#include <utility>     // since I am using make_pair

using std::invalid_argument;
using std::pair;
using std::string;

// Function to convert a string like "D5" into numerical coordinates (row, column)
pair<int, int> convertToCoordinates(const string& input) {
    if (input.length() < 2) { // Checks if input is less than 2 characters long which would make it invaid
        throw invalid_argument("Invalid input format. Must be in the format of LetterNumber, e.g., 'D5'.");
    }

    // Convert column letter (A-J) to a column index (0-9)
    char columnLetter = toupper(input[0]);  // Ensure the letter is uppercase
    if (columnLetter < 'A' || columnLetter > 'J') { // Checks if the letter is within the range of the table
        throw invalid_argument("Invalid column letter. Must be between A and J.");
    }
    int column = columnLetter - 'A';  // 'A' becomes 0, 'B' becomes 1, ..., 'J' becomes 9

    // Convert row number (1-10) to a row index (0-9)
    int row = stoi(input.substr(1));  // Extract the number part of the input (stoi()- converts the substring to an integer)
    if (row < 1 || row > 10) { // Cheks if the number is in the valid range of the table
        throw invalid_argument("Invalid row number. Must be between 1 and 10.");
    }
    row--;  // Convert to 0-based index

    return std::make_pair(row, column); // returns a pair of coordinates (pair <int, int>)
}