// noashalom5@gmail.com
#include "game.hpp"
#include "player.hpp"
#include "logger.hpp" 
#include <algorithm>
#include <iostream>
#include <stdexcept> 
#include <string>
using namespace std;

namespace coup {

    bool Player::myTurn() const {
        string userTurn = game->currentPlayer()->getUsername();
        if(userTurn != username) {
            Logger::getInstance().log("Error: It's not your turn!");
            return false;
        }
        return true;
    }

    bool Player::tenCoins() const {
        if(coins >= 10) {
            Logger::getInstance().log("Error: You have 10 coins or more. you must performed coup."); 
            return true;
        }
        return false;
    }

    bool Player::isAlive() const {
        const auto& players = game->getPlayersList();
        if (find(players.begin(), players.end(), this) == players.end()) {
            Logger::getInstance().log("Error: The player was eliminated from the game.");
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
        Logger::getInstance().log(username + " skipped his turn.");
    }

    void Player::gather() {
        if(!game->getGameStarted()) game->setGameStarted(true);
        if(!myTurn()) return;
        if(tenCoins()) return;
        if(underSanction && getRole() != "Baron") {
            Logger::getInstance().log("Error: You can't performe gather. you are under sanction. pick another action.");
            return;
        }
        coins++;
        game->moveTurn();
        lastAction = "gather";
        if(cantDoArrest)
            cantDoArrest = false;
        Logger::getInstance().log(username + " performed gather.");
    }

    void Player::tax() {
        if(!game->getGameStarted()) game->setGameStarted(true);
        if(!myTurn()) return;
        if(tenCoins()) return;
        if(underSanction && getRole() != "Baron") {
            Logger::getInstance().log("Error: You can't performe tax. you are under sanction. pick another action.");
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
                Logger::getInstance().log("You are under sanction. you will only receive one coin as compensation.");
                coins++;
                game->moveTurn();
                Logger::getInstance().log(username + " performed tax.");
                return;
            }
        }
        
        if(block) {
            Logger::getInstance().log(username + " was blocked from committing a tax.");
            return;
        }

        if(getRole() == "Governor")
            coins += 3;
        else
            coins += 2;
        game->moveTurn();
        Logger::getInstance().log(username + " performed tax.");
    }

    void Player::bribe() {
        if(!myTurn()) return;
        if(tenCoins()) return;
        if(coins < 4) {
            Logger::getInstance().log("Error: You don't have enough coins to performed bribe! pick another action.");
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
            Logger::getInstance().log(username + " was blocked from committing a bribe.");
            return;
        }
        Logger::getInstance().log(username + " performed bribe.");
    }

    void Player::arrest(Player* p) {
        if (p == nullptr) return;

        if (!game->getGameStarted()) game->setGameStarted(true);
        if (!myTurn()) return;
        if (tenCoins()) return;
        if (!p->isAlive()) return;
        if (cantDoArrest) {
            Logger::getInstance().log("Error: You cannot commit arrest this turn! Pick another action.");
            return;
        }
        if (didArrest == p->getUsername()) {
            Logger::getInstance().log("Error: You cannot arrest " + p->getUsername() + " again. Pick another player.");
            return;
        }
        if (p->getRole() == "Merchant") {
            int coinsMerchant =  p->getCoins();
            if (coinsMerchant >= 2) {
                p->setCoins(coinsMerchant-2);
                Logger::getInstance().log(username + " arrested a merchant.");
                didArrest = p->getUsername();
                game->moveTurn();
                lastAction = "arrest";
                return;
            }
        }
        if (p->getRole() != "General") {
            if (p->getCoins() == 0) {
                Logger::getInstance().log("You cannot commit arrest on this player, they don't have enough coins. Pick another action or player.");
                return;
            }
            p->setCoins(p->getCoins()-1);
            coins++;
        }
        didArrest = p->getUsername();
        game->moveTurn();
        lastAction = "arrest";
        Logger::getInstance().log(username + " performed arrest.");
    }

    void Player::sanction(Player* p) {
        if (p == nullptr) return;

        if (!myTurn()) return;
        if (tenCoins()) return;
        if (!p->isAlive()) return;
        if (coins < 3) {
            Logger::getInstance().log("Error: You don't have enough coins to perform sanction! Pick another action.");
            return;
        }
        if (p->getRole() == "Judge") {
            if (coins < 4) {
                Logger::getInstance().log("Error: You don't have enough coins to perform sanction! Pick another action.");
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
        Logger::getInstance().log(username + " performed sanction.");
    }

    void Player::coup(Player* p) {
        if (p == nullptr) return;

        if (!myTurn()) return;
        if (!p->isAlive()) return;
        if (coins < 7) {
            Logger::getInstance().log("Error: You don't have enough coins to perform coup! Pick another action.");
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
            Logger::getInstance().log(username + " was blocked from committing a coup.");
            return;
        }

        Logger::getInstance().log(username + " performed coup!");
        Logger::getInstance().log(p->getUsername() + " is out of the game!");
        game->removePlayer(p->getUsername());
        game->moveTurn();
    }

}
