#include "Board.h"

Board::Board(){ // Constructor to initialize the boards
    for (int i = 0; i < 10; i++) { // The loops go through each row and column and mark them as '_'
        for (int j = 0; j < 10; j++) {
            shipBoard[i][j] = '_';
            attackBoard[i][j] = '_';
        }
    }
}; 

void Board::displayBoard(DisplayMode mode) { // Displays the board based on the specified DisplayMode
    // Display column letters above both boards
    if (mode == BOTH) {
        cout << "| _ | A | B | C | D | E | F | G | H | I | J |";  // Letters for ship board
        cout << "        ";  // Spacing between the two boards
        cout << "| _ | A | B | C | D | E | F | G | H | I | J |" << endl;  // Letters for attack board
    } else {
        // Display letters for only one board (ship or attack)
        cout << "| _ | A | B | C | D | E | F | G | H | I | J |" << endl;
    }
    if (mode == BOTH || mode == SHIP_BOARD){
        for (int row = 0; row < 10; row++) {
            // Display the row number and shipBoard for the current row
            cout << (row < 9 ? "| " : "|") << row + 1 << " |";  // Row number (1-based)
            for (int col = 0; col < 10; col++) {
                if (mode == SHIP_BOARD || mode == BOTH) {
                    cout << " " << shipBoard[row][col] << " |";
                }
            }
            // If displaying BOTH boards, add spacing and then display the attackBoard on the same line
            if (mode == BOTH) {
                cout << "        ";  // Space between shipBoard and attackBoard
                cout << (row < 9 ? "| " : "|") << row + 1 << " |";  // Row number for attackBoard
                for (int col = 0; col < 10; col++) {
                    cout << " " << attackBoard[row][col] << " |";
                }
            }
            // End the line for the current row
            cout<< endl;
        }
    } else { // If only displaying the attack board
        for (int row = 0; row < 10; row++) {
            cout << (row < 9 ? "| " : "|") << row + 1 << " |";  // Row number
            for (int col = 0; col < 10; col++) {
                cout << " " << attackBoard[row][col] << " |";
            }
            cout<< endl;
        }
    }
    cout<<endl;
}

// Method to check if a ship can be placed at the given position
bool Board::canPlaceShip(int x, int y, int size, char orientation) {
    if (orientation == 'H') { // Horizontal placement
        for (int i = 0; i < size; ++i) {
            if (x >= 0 && x < 10 && (y + i) >= 0 && (y + i) < 10) { // Check if a ship is within boundaries
                if (shipBoard[x][y + i] != '_') {  // Check if the position is already occupied
                    return false; // Position is occupied
                }
            } else {
                return false; // Out of bounds
            }
        }
    } else if (orientation == 'V') { // Vertical placement
        for (int i = 0; i < size; ++i) {
            if ((x + i) >= 0 && (x + i) < 10 && y >= 0 && y < 10) { // Check if a ship is within boundaries
                if (shipBoard[x + i][y] != '_') {  // Check if the position is already occupied
                    return false; // Position is occupied
                }
            } else {
                return false; // Out of bounds
            }
        }
    }
    return true; // All positions are free
}
 
// Method to place a ship on the board and then return the coordinates of each ship fragment inside a vector
vector<pair<int, int>> Board::placeShip(int x, int y, int size, char orientation, char symbol) {
    vector<pair<int, int>> coordinates;

    if (orientation == 'H') { // Horizontal placement
        for (int i = 0; i < size; ++i) {
            shipBoard[x][y + i] = symbol;
            coordinates.push_back({x, y + i});
        }
        //cout << "Horizontal ship placed at (" << x + 1 << "," << y + 1 << ")" << endl; // Uncomment to see ship placement for player and AI
    } else if (orientation == 'V') { // Vertical placement
        for (int i = 0; i < size; ++i) {
            shipBoard[x + i][y] = symbol;
            coordinates.push_back({x + i, y});
        }
        //cout << "Vertical ship placed at (" << x + 1 << "," << y + 1 << ")" << endl;
    }
    return coordinates;
}

// marks the attack board
void Board::mark_attack(int x, int y, bool isHit) {
    attackBoard[x][y] = isHit ? 'X' : 'O';
}

// used to mark opponents ship board
void Board::mark_attack_ship(int x, int y, bool isHit) {
    shipBoard[x][y] = isHit ? 'X' : 'O';
}
