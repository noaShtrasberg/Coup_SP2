// noashalom5@gmail.com
#include "game.hpp"
#include "player.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept> 
#include <string>
using namespace std;

namespace coup {

    // Check if this is player's turn
    bool Player::myTurn() const {
        string userTurn = game->currentPlayer()->getUsername();
        if(userTurn != username) {
            cout << "Error: It's not your turn!" << endl;
            return false;
        }
        return true;
    }

    bool Player::tenCoins() const {
        if(coins >= 10) {
            cout << "Error: You have 10 coins or more. you must performed coup." << endl; 
            return true;
        }
        return false;
    }

    bool Player::isAlive() const {
        const auto& players = game->getPlayersList();
        if (find(players.begin(), players.end(), this) == players.end()) {
            cout << "Error: The player was eliminated from the game." << endl;
            return false;
        }
        return true;
    }

    void Player::skipTurn() {
        if(!myTurn()) return;
        if(tenCoins()) return;
        game->moveTurn();
        if(cantDoArrest)
            cantDoArrest = false;
        cout << username << " skipped his turn." << endl;
    }

    void Player::gather() {
        if(!game->getGameStarted()) game->setGameStarted(true);
        if(!myTurn()) return;
        if(tenCoins()) return;
        if(underSanction) {
            if(getRole() != "Baron") {
                cout << "Error: You can't performe gather. you are under sanction. pick another action." << endl;
                return;
            }
        }
        coins++;
        game->moveTurn();
        lastAction = "gather";
        if(cantDoArrest)
            cantDoArrest = false;
        cout << username << " performed gather." << endl;
    }

    void Player::tax() {
        if(!game->getGameStarted()) game->setGameStarted(true);
        if(!myTurn()) return;
        if(tenCoins()) return;
        if(underSanction && getRole() != "Baron") {
                cout << "Error: You can't performe tax. you are under sanction. pick another action." << endl;
                return;
        }

        bool block = false;
        if(game->thereIs("Governor")) {
            block = game->undoTax();
        }
        lastAction = "tax";
        if(cantDoArrest)
            cantDoArrest = false;
        
        if(underSanction) { // This is a Baron
            underSanction = false;
            if(!block) {
                cout << "You are under sanction. you will only receive one coin as compensation." << endl;
                coins++;
                game->moveTurn();
                cout << username << " performed tax." << endl;
                return;
            }
        }
        
        if(block) {
            cout << username << " was blocked from committing a tax." << endl;
            return;
        }

        if(getRole() == "Governor")
            coins += 3;
        else
            coins += 2;
        game->moveTurn();
        cout << username << " performed tax." << endl;
    }

    void Player::bribe() {
        if(!myTurn()) return;
        if(tenCoins()) return;
        if(coins < 4) {
            cout << "Error: You don't have enough coins to performed bribe! pick another action." << endl;
            return;
        }
        coins -= 4;

        bool block = false;
        if(game->thereIs("Judge")) {
            block = game->undoBribe();
        }
        lastAction = "bribe";
        if(cantDoArrest)
            cantDoArrest = false;

        if(block) {
            cout << username << " was blocked from committing a bribe." << endl;
            return;
        }
        cout << username << " performed bribe." << endl;
    }

    void Player::arrest(Player* p) {
        if (p == nullptr) return;

        if (!game->getGameStarted()) game->setGameStarted(true);
        if (!myTurn()) return;
        if (tenCoins()) return;
        if (!p->isAlive()) return;
        if (cantDoArrest) {
            cout << "Error: You cannot commit arrest this turn! Pick another action." << endl;
            return;
        }
        if (didArrest == p->getUsername()) {
            cout << "Error: You cannot arrest " << p->getUsername() << " again. Pick another player." << endl;
            return;
        }
        if (p->getRole() == "Merchant") {
            int coinsMerchant =  p->getCoins();
            if (coinsMerchant >= 2) {
                p->setCoins(coinsMerchant-2);
                cout << username << " arrested a merchant." << endl;
                didArrest = p->getUsername();
                game->moveTurn();
                lastAction = "arrest";
                return;
            }
        }
        if (p->getRole() != "General") {
            if (p->getCoins() == 0) {
                cout << "You cannot commit arrest on this player, they don't have enough coins. Pick another action or player." << endl;
                return;
            }
            p->setCoins(p->getCoins()-1);
            coins++;
        }
        didArrest = p->getUsername();
        game->moveTurn();
        lastAction = "arrest";
        cout << username << " performed arrest." << endl;
    }

    void Player::sanction(Player* p) {
        if (p == nullptr) return;

        if (!myTurn()) return;
        if (tenCoins()) return;
        if (!p->isAlive()) return;
        if (coins < 3) {
            cout << "Error: You don't have enough coins to perform sanction! Pick another action." << endl;
            return;
        }
        if (p->getRole() == "Judge") {
            if (coins < 4) {
                cout << "Error: You don't have enough coins to perform sanction! Pick another action." << endl;
                return;
            }
            coins -= 4;
        } else {
            coins -= 3;
        }

        p->underSanction = true;
        game->moveTurn();
        lastAction = "sanction";
        if (cantDoArrest)
            cantDoArrest = false;
        cout << username << " performed sanction." << endl;
    }

    void Player::coup(Player* p) {
        if (p == nullptr) return;

        if (!myTurn()) return;
        if (!p->isAlive()) return;
        if (coins < 7) {
            cout << "Error: You don't have enough coins to perform coup! Pick another action." << endl;
            return;
        }
        coins -= 7;

        bool block = false;
        if (game->thereIs("General")) {
            block = game->undoCoup();
        }
        lastAction = "coup";
        if (cantDoArrest)
            cantDoArrest = false;

        if (block) {
            cout << username << " was blocked from committing a coup." << endl;
            return;
        }

        cout << username << " performed coup!" << endl;
        cout << p->getUsername() << " is out of the game!" << endl;
        game->removePlayer(p->getUsername());
        game->moveTurn();
    }

}