#include "Ship.h"

Ship::Ship(string shipName, int shipSize): name(shipName), size(shipSize){}
    
// Function to set the coordinates based on board placement
void Ship::setCoordinates(const vector<pair<int, int>>& newCoordinates) {
    coordinates = newCoordinates;
}

// Check if the ship is hit at the given coordinates
bool Ship::is_hit(int x, int y) {
    for (const auto& coord : coordinates) {
        if (coord.first == x && coord.second == y) {
            return true;
        }
    }
    return false;
}

// Register a hit at the given coordinates by marking it as (-1, -1)
void Ship::register_hit(int x, int y) {
    for (auto& coord : coordinates) {
        if (coord.first == x && coord.second == y) {
            coord = {-1, -1};  // Mark fragment as hit
            break;
        }
    }
}

// Check if all ship fragments are hit and set sunk to true if so
bool Ship::is_sunk() const {
    for (const auto& coord : coordinates) {
        if (coord.first != -1 || coord.second != -1) {
            return false;  // Ship is not fully sunk if any remaining fragment isn't hit
        }
    }
    return true;
}

string Ship::ship_name() const{
    return name;
}

int Ship::ship_size() const{
    return size;
}