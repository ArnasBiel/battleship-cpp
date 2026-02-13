#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <cstdlib> // For rand and srand
#include <ctime>   // For seeding rand()
#include <utility> // Including just in case since I am using make_pair

#include "Extras.h"
#include "Board.h"
#include "Ship.h"
#include "Player.h"
#include "AiPlayer.h"
 
using namespace std;

void gameresults(int PMoves, int AIMoves, int PTurns){
    cout << "Turns taken: "<< PTurns<<endl;
    cout << "Player moves: "<< PMoves<< " A.Smith's moves: "<< AIMoves<<endl;
    cout <<"Total moves taken: "<< PMoves + AIMoves<< endl;
}

void gameOrder() {
    Player player;
    AIPlayer aiPlayer;

    int playerMoves = 0;
    int AiMoves = 0;
    int playerTurns = 0;

    // Board setup for the Player and AI
    player.placeAllShips();
    player.display_board();
    aiPlayer.placeAllShips();
    //aiPlayer.display_board(); // Uncomment if you want to see how the AI placed their ships

    // Game loop
    while (player.ships_remaining() > 0 && aiPlayer.ships_remaining() > 0) {
        // Player's turn
        bool playerTurn = true;
        playerTurns++;
        while (playerTurn && aiPlayer.ships_remaining() > 0) {
            cout<<"***************************************************************************************************"<<endl;
            cout << "Player's turn:" << endl;
            playerMoves++; // counts how many moves a player did
            playerTurn = player.getAttackCoordinates(aiPlayer);  // Player continues if they hit
            if (aiPlayer.ships_remaining() == 0) {
                cout << "Player wins!" << endl;
                gameresults(playerMoves, AiMoves, playerTurns); // print out game results
                return;
            }
        }

        // AI's turn
        bool aiTurn = true;
        aiPlayer.heatInitialization(); // updates the heatmap with all possible ship positions before each turn 
        // this way if there are multiple misses, the heatmap starts adding more weight diagonaly going from the middle 
        //(it was not programmed specifically as it is an emergent property of having higher weights in the middle at the start, as a result of all possible ship orienations)
        while (aiTurn && player.ships_remaining() > 0) {
            cout<<"***************************************************************************************************"<<endl;
            cout << "A.Smith's turn:" << endl;
            AiMoves++; // counts how many moves an AI did
            aiTurn = aiPlayer.getAttackCoordinates(player);  // AI continues if they hit
            if (player.ships_remaining() == 0) {
                cout << "A.Smith wins!" << endl;
                gameresults(playerMoves, AiMoves, playerTurns); // print out game results
                return;
            }
        }
    }
}

int main() {
    gameOrder();
    return 0;
}