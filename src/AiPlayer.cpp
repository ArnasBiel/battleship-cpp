#include "AiPlayer.h"

AIPlayer::AIPlayer() {
    srand(time(0));  // Seed for randomness once in the constructor
}
    
// Method to check if a ship can be placed at the given position for the heatmap (the same as canPlaceShip in Board but with diferent variables). 
//It also checks the number of hits in the place where the ship can be placed, that ship placement has a higher weight to encourage attacking that placement
pair<bool, int> AIPlayer::shipCheck(int x, int y, int size, char orientation) {
    int has_hit = 0;

    if (orientation == 'H') { // Horizontal placement
        for (int i = 0; i < size; ++i) {
            if (x >= 0 && x < 10 && (y + i) >= 0 && (y + i) < 10) { // Check if within boundaries
                if (heatmap[x][y + i] < -1) { // If cell was missed (-2), cannot place ship here
                    return {false, 0};
                }
                if (heatmap[x][y + i] == -1) { // Check if there's a hit (-1) in the range
                    has_hit ++;
                }
            } else {
                return {false, 0}; // Out of bounds
            }
        }
    } else if (orientation == 'V') { // Vertical placement
        for (int i = 0; i < size; ++i) {
            if ((x + i) >= 0 && (x + i) < 10 && y >= 0 && y < 10) { // Check if within boundaries
                if (heatmap[x + i][y] < -1) { // If cell was missed (-2), cannot place ship here
                    return {false, 0};
                }
                if (heatmap[x + i][y] == -1) { // Check if there's a hit (-1) in the range
                    has_hit ++;
                }
            } else {
                return {false, 0}; // Out of bounds
            }
        }
    }
    return {true, has_hit}; // Placement is possible, and indicates if there's a hit in the range
}

void AIPlayer::heatInitialization() { // goes through all the ships and all their possible positions to create and update the heatmap
    for (auto& ship : fleet) {
        int ship_size = ship.ship_size(); // Get the ship size

        // Loop over all possible coordinates
        for (int x = 0; x < 10; x++) { // row
            for (int y = 0; y < 10; y++) { // column
                // Check if a ship can be placed horizontally
                auto [can_place_h, hit_count_h] = shipCheck(x, y, ship_size, 'H');
                if (can_place_h) {
                    int weight_increment = 1 + hit_count_h * 2; //The weight increases with every hit in that position (0 hits +1; 1 hit +3; 2 hits +5...)
                    for (int i = 0; i < ship_size; i++) {
                        if (heatmap[x][y + i] != -1) { // Leave the hit value the same for visualization purposes
                            heatmap[x][y + i] += weight_increment; // Increase by +2 if there's a hit in that position, otherwise +1
                        }
                    }
                }
                    
                // Check if a ship can be placed vertically
                auto [can_place_v, hit_count_v] = shipCheck(x, y, ship_size, 'V');
                if (can_place_v) {
                    int weight_increment = 1 + hit_count_v * 2;
                    for (int i = 0; i < ship_size; i++) {
                        if (heatmap[x + i][y] != -1) { // Leave the hit value the same for visualization purposes
                            heatmap[x + i][y] += weight_increment; // Increase by +2 if there's a hit, otherwise +1
                        }
                    }
                }
            }
        }
    }
}

void AIPlayer::display_heatmap(){
    // Check to see how the heatmap looks
    for (int x=0; x<10; x++){
        for (int y=0; y<10; y++){
            cout<<heatmap[x][y]<<" ";
        }
        cout<< endl;
    }
    cout<< endl;
}

pair<int, int> AIPlayer::heatShot(){ // finds the highest value on the heatmap to take a shot
    int max_value = heatmap[0][0];
    int best_x = 0;
    int best_y = 0;

    // Iterate through the entire heatmap to find the highest value 
    // (since it checks the columns for every row, the cells with the same max value that are more to the upper left side are chosen first)
    for (int x = 0; x<10; x++){
        for (int y = 0; y<10; y++){
            if (heatmap[x][y]>max_value){
                max_value = heatmap[x][y];
                best_x = x;
                best_y = y;
            }
        }
    }
    return {best_x, best_y}; // coordinates with the highest heat value
}

void AIPlayer::updateHeat(int x, int y, bool hit_status, bool was_sunk) {
    if (hit_status) {
        heatmap[x][y] = attack_hit; // marks the coordinate as a hit (-1)
        if (!was_sunk){ // only update arround hit if there are still ship fragments left (the ship has not been sunk yet)
            // Direction array for neighboring cells: left, right, down, up
            int directions[4][2] = {{0, -1}, {0, 1}, {1, 0}, {-1, 0}};
            bool surrounding_hit_found = false;

            // Iterate through each direction to check for adjacent and distant hits
            for (const auto& dir : directions) {
                int adj_x = x + dir[0];
                int adj_y = y + dir[1];

                // Check if there's an adjacent hit in the current direction
                if (adj_x >= 0 && adj_x < 10 && adj_y >= 0 && adj_y < 10 && heatmap[adj_x][adj_y] == attack_hit) {
                    surrounding_hit_found = true;

                    // Calculate the position opposite to the adjacent hit
                    int opposite_x = x - dir[0];
                    int opposite_y = y - dir[1];

                    // First, try to increase the heatmap value on the opposite side of a previous hit (next to the current hit)
                    if (opposite_x >= 0 && opposite_x < 10 && opposite_y >= 0 && opposite_y < 10 && heatmap[opposite_x][opposite_y] >= 0) {
                        heatmap[opposite_x][opposite_y] += 15; // Increase the heatmap for the opposite cell
                    } else {
                        // If opposite cell is not available (out of bounds or already attacked), increase the adjacent cell on the side of the previous hit (two cells away from the current hit)
                        int new_x = x + 2 * dir[0];
                        int new_y = y + 2 * dir[1];
                            
                        if (new_x >= 0 && new_x < 10 && new_y >= 0 && new_y < 10 && heatmap[new_x][new_y] >= 0) {
                            heatmap[new_x][new_y] += 15; // Increase the heatmap for two cells away on the hit side
                        }
                    }
                }

                // Check for a hit two cells away to create a bridge effect between hits
                int distant_x = x + 2 * dir[0];
                int distant_y = y + 2 * dir[1];

                if (distant_x >= 0 && distant_x < 10 && distant_y >= 0 && distant_y < 10 && heatmap[distant_x][distant_y] == attack_hit) {
                    surrounding_hit_found = true;

                    // Increase the values in the cell between the hits (the cell adjacent to x, y in this direction)
                    if (heatmap[adj_x][adj_y] >= 0) { // The cell between has not been shot at before
                        heatmap[adj_x][adj_y] += 15; // Increase the heatmap for the cell between the hits
                    }
                }
            }

            // If there were no surrounding hits, increase values in 4 directions by 5
            if (!surrounding_hit_found) {
                for (const auto& dir : directions) {
                    int new_x = x + dir[0];
                    int new_y = y + dir[1];
                        
                    if (new_x >= 0 && new_x < 10 && new_y >= 0 && new_y < 10) {
                        if (heatmap[new_x][new_y] >= 0) { // Ensure it's not marked as a hit/miss
                            heatmap[new_x][new_y] += 10;
                        }
                    }
                }
            }
        }
    } else {
        heatmap[x][y] = attack_missed;
    }
}

// AI will place ships randomly
void AIPlayer::placeAllShips(){
    for (auto& ship : fleet) {
        bool placed = false;
        while (!placed) {
            int x = rand() % 10;
            int y = rand() % 10;
            char orientation = rand() % 2 == 0 ? 'H' : 'V';  // Random orientation
 
            // Check if the ship can be placed at the given coordinates and orientation
            if (board.canPlaceShip(x, y, ship.ship_size(), orientation)) {
                vector<pair<int, int>> coordinates = board.placeShip(x, y, ship.ship_size(), orientation, ship.ship_name()[0]);
                ship.setCoordinates(coordinates);  // Directly pass coordinates to setCoordinates
                placed = true;
            }
        }
    }
    cout<< endl;
    cout << "A.Smith placed all ships." << endl;
    cout<< endl;
}

bool AIPlayer::getAttackCoordinates(Player &opponent){ // Reference to the original Player object that let's directly eddit it without the copy
    bool valid_attack = false;
    cout<<"***************************************************************************************************"<<endl;
    cout<<"Current A.Smith's attack board: "<< endl;
    board.displayBoard(Board::ATTACK_BOARD);
    // display_heatmap(); // Uncomment to see the heatmap before the shot
    while (!valid_attack) {
        pair<int, int> shot = heatShot();
        // Extract x and y from the pair
        int x = shot.first;
        int y = shot.second;

        if (board.attackBoard[x][y] == '_') { // if the spot has not been attacked before
            auto [hit, sunk] = processAttack(x, y, opponent); // Call processAttack with opponent
            updateHeat(x, y, hit, sunk); // Change the values around the hit in the heatmap
            // Print the appropriate message
            if (hit) {
                if (!sunk) {
                    cout << "A.Smith Hit!" << endl;
                }
            } else {
                cout << "A.Smith Miss!" << endl;
            }
            cout << "A.Smith's attack board after the attack: "<<endl;
            board.displayBoard(Board::ATTACK_BOARD);
            valid_attack = true;
            // display_heatmap(); // Uncomment to see the heatmap after the shot
            if (sunk){
                return false; // According to the rules "a player continues shooting on a hit until they miss or destroy a ship", so stop the loop after sinking a ship
            }
            return hit;  // Return true if it was a hit for later to manage the continuous attack after hits
        }
    }
    return false;
}