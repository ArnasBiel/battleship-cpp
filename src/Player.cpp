#include "Player.h"
#include "Extras.h"

Player::Player() {
    initializeFleet();  // Initialize the fleet with ships
}

// Initialize the fleet with the predefined ships and sizes (using vector so that new ships could be added)
void Player::initializeFleet() {
    fleet.push_back(Ship("Carrier", 5));
    fleet.push_back(Ship("Battleship", 4));
    fleet.push_back(Ship("Cruiser", 3));
    fleet.push_back(Ship("Submarine", 3));
    fleet.push_back(Ship("Destroyer", 2));
}

// Interactive function to place all ships on the board
void Player::placeAllShips() {
    for (Ship &ship : fleet) { // Goes through each ship in the fleet
        bool placed = false;

        while (!placed) {
            board.displayBoard(Board::SHIP_BOARD); // Display board for easier placement
            cout << "Placing " << ship.ship_name() << " (Size: " << ship.ship_size() << ")" << endl;
            cout << "Enter starting coordinates (column and row)(e.g., D5): ";
                
            string input;
            cin >> input;

            // Convert the input string (e.g., "D5") to coordinates
            pair<int, int> coordinates;
            try {
                coordinates = convertToCoordinates(input);  // Call the conversion function
            } catch (const invalid_argument& e) {
                cout << e.what() << endl;
                continue;  // If the input was invalid, try again
            }

            int x = coordinates.first;
            int y = coordinates.second;

            cout << "Enter orientation (H for horizontal, V for vertical): ";
            char orientation;
            cin >> orientation;
            orientation = toupper(orientation); // Ensure uppercase input
            // Check if the ship can be placed at the given coordinates and orientation
            if (board.canPlaceShip(x, y, ship.ship_size(), orientation)) {
                vector<pair<int, int>> coordinates = board.placeShip(x, y, ship.ship_size(), orientation, ship.ship_name()[0]);
                ship.setCoordinates(coordinates);  // Directly pass coordinates to setCoordinates wich saves them for each specific ship fragment
                placed = true;
            } else {
                cout << "Invalid placement. Please try again." << endl;
            }
        }
    }
}

// Get attack coordinates from player and execute attack on opponent
bool Player::getAttackCoordinates(Player &opponent) {
    bool valid_attack = false;
    cout<<"***************************************************************************************************"<<endl;
    cout<<"Current boards: "<< endl;
    display_board();
    while (!valid_attack) {
        cout << "Enter attack coordinates (e.g., D5): ";
        string attack_input;
        cin >> attack_input;

        pair<int, int> coordinates;
        try {
            coordinates = convertToCoordinates(attack_input); // Converts into numerical coordinates
        } catch (const invalid_argument& e) { //catches the error message from convertToCoordinates
            cout << e.what() << endl; // prints out the error message
            continue; // restarts the loop for a new attack input
        }

        int x = coordinates.first;
        int y = coordinates.second;

        if (board.attackBoard[x][y] == '_') { // if the spot has not been attacked before
            auto [hit, sunk] = processAttack(x, y, opponent); // Call processAttack with opponent
            // Print the appropriate message
            if (hit) {
                if (!sunk) {
                    cout << "Hit!" << endl;
                }
            } else {
                cout << "Miss!" << endl;
            }
            board.displayBoard(Board::ATTACK_BOARD);
            valid_attack = true;
            if (sunk){
                return false; // According to the rules "a player continues shooting on a hit until they miss or destroy a ship", so stop the loop after sinking a ship
            }
            return hit;  // Return true if it was a hit for later to manage the continuous attack after hits
        } else {
            cout << "This location has already been attacked. Try again." << endl;
        }
    }
    return false; // rteurn false if it is a miss so that it would be an opponents turn next
}

pair <bool, bool> Player::processAttack(int x, int y, Player &opponent) { // It is basically a shootAt function that checks for hits and if the ship has been sunk
    for (Ship &ship : opponent.fleet) {  // Access opponent's fleet ship by ship
        if (ship.is_hit(x, y)) { //sends coordinates to is_hit to check if any ship fragments of the opponent ships are in those coordinates
            board.mark_attack(x, y, true);  // Mark the hit on attack board
            opponent.board.mark_attack_ship(x, y, true); // marks the opponent ship board with a hit
            ship.register_hit(x, y); // marks the ship fragment with register_hit as a hit which is important for tracking if a ship has been sunk

            bool ship_sunk = false; // represents the state of the ship
            if (ship.is_sunk()) {
                opponent.shipsRemaining--;
                cout << "The " << ship.ship_name() << " has been sunk!" << endl;
                cout << "Ships remaining: "<< opponent.shipsRemaining<<endl;
                ship_sunk = true;
            }
            return {true, ship_sunk};  // Return true to indicate a successful hit and if the ship has been sunk or not
        }
    }
    board.mark_attack(x, y, false);  // Mark a miss on attack board
    opponent.board.mark_attack_ship(x, y, false); // Mark miss on opponent ship board
    return {false, false};  // No ship was hit
}

int Player::ships_remaining() const{
    return shipsRemaining;
}

// Display the player's boards
void Player::display_board() {
    board.displayBoard(Board::BOTH);
}
