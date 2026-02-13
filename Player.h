#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <string>
#include "Board.h"
#include "Ship.h"

using std::vector;
using std::pair;
using std::cout;
using std::cin;
using std::endl;


class Player {
private:
    int shipsRemaining = 5;  // The player loses when this hits 0
public:
    bool currentTurn = false;
    Board board;
    vector<Ship> fleet;
    Player();  
    void initializeFleet(); 
    virtual void placeAllShips();
    virtual bool getAttackCoordinates(Player &opponent); 
    pair <bool, bool> processAttack(int x, int y, Player &opponent);
    int ships_remaining() const;
    void display_board();
};

#endif //PLAYER_H