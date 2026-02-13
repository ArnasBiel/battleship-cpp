#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <iostream>
#include <vector>
#include <string>
#include "Board.h"
#include "Ship.h"
#include "Player.h"

using std::vector;
using std::pair;
using std::cout;
using std::endl;

class Player;

class AIPlayer : public Player {
private:
    int heatmap[10][10] = {0};
    int attack_hit = -1;   // Give different values for hits and misses to make heatmap interpretability easier
    int attack_missed = -2;
public:
    AIPlayer();
    
    pair<bool, int> shipCheck(int x, int y, int size, char orientation);
    void heatInitialization();
    void display_heatmap();
    pair<int, int> heatShot();
    void updateHeat(int x, int y, bool hit_status, bool was_sunk);
    void placeAllShips() override;
    bool getAttackCoordinates(Player &opponent) override;
};

#endif //AIPLAYER_H