// noashalom5@gmail.com

#include "baron.hpp"
#include "game.hpp"
#include <iostream>

using namespace std;

namespace coup {
    void Baron::specialTurn() {
        this->invest();
    }

    void Baron::invest() {
        if(!myTurn()) return;
        if(tenCoins()) return;
        if(coins < 3) {
            cout << "Error: You don't have enough coins to performed invest! pick another action." << endl;
            return;
        }
        coins += 3;
        lastAction = "invest";
        game->moveTurn();
        if(actionBribe == 1) actionBribe--;        
    }
}