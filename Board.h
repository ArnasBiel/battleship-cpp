#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::pair;
using std::cout;
using std::endl;

class Board {
public:
    char shipBoard[10][10];  // Ship placement board
    char attackBoard[10][10]; // Attack marking board
    Board(); 

    // Enum to make display mode more readable
    enum DisplayMode {
        SHIP_BOARD,  // Show the ship placement board
        ATTACK_BOARD, // Show the attack marking board
        BOTH // Show both boards side by side
    };

    void displayBoard(DisplayMode mode);// Displays the board based on the specified DisplayMode
    bool canPlaceShip(int x, int y, int size, char orientation);// Method to check if a ship can be placed at the given position
    vector<pair<int, int>> placeShip(int x, int y, int size, char orientation, char symbol); // Method to place a ship on the board and then return the coordinates of each ship fragment inside a vector
    void mark_attack(int x, int y, bool isHit);// marks the attack board
    void mark_attack_ship(int x, int y, bool isHit);// used to mark opponents ship board
};

#endif //BOARD_H