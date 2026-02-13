#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <string>

using std::vector;
using std::pair;
using std::string;

class Ship {
private:
    vector<pair<int, int>> coordinates; // Track ship position on the board
    string name;
    int size;
public:
    Ship(string shipName, int shipSize);

    void setCoordinates(const vector<pair<int, int>>& newCoordinates);
    bool is_hit(int x, int y);
    void register_hit(int x, int y);
    bool is_sunk() const;
    string ship_name() const;
    int ship_size() const;
};

#endif //SHIP_H