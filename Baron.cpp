// noashalom5@gmail.com
#include <iostream>
#include <stdexcept> 
#include <string>
using namespace std;

namespace coup {
    void Baron::specialTurn() {
        this->invest();
    }

    void Baron::invest() const {
        if(!this->myTurn()) return;
        if(this->tenCoins()) return;
        if(coins < 3) {
            cout << "Error: You don't have enough coins to performed invest! pick another action." << endl;
            return;
        }
        coins += 3;
        game->moveTurn();
        lastAction = "invest";
    }
}