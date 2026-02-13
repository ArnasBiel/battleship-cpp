#ifndef EXTRAS_H
#define EXTRAS_H

#include <cctype>      // For toupper
#include <stdexcept>   // For std::invalid_argument
#include <string>      // For std::string
#include <iostream>    // If needed for debugging purposes
#include <utility>     // since I am using make_pair

using std::invalid_argument;
using std::pair;
using std::string;

pair<int, int> convertToCoordinates(const string& input);

#endif //Extras_H