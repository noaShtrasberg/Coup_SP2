// noashalom5@gmail.com

#include "spy.hpp"
#include "player.hpp"
#include "game.hpp"
#include <iostream>

using namespace std;

namespace coup {
    void Spy::specialTurn() {
        cout << "Spy special turn - choose an action:\n";
        cout << "1. See coins of another player\n";
        cout << "2. Block arrest of another player\n";
        cout << "Enter your choice (1/2): ";
        int choice;
        cin >> choice;

        if (choice == 1) { // seeCoins
            string targetName;
            cout << "Enter target player's username: ";
            cin >> targetName;
            Player* target = game->findPlayerByUsername(targetName);
            if (target == nullptr) {
                cout << "Error: Player not found." << endl;
                return;
            }
            this->seeCoins(target);
        }
        else if (choice == 2) { // blockArrest
            string targetName;
            cout << "Enter target player's username: ";
            cin >> targetName;
            Player* target = game->findPlayerByUsername(targetName);
            if (target == nullptr) {
                cout << "Error: Player not found." << endl;
                return;
            }
            this->blockArrest(target);
        }
    }

    void Spy::seeCoins(Player* p) {
        if(!p->isAlive()) return;
        if (username == p->getUsername()) {
            cout << "Cannot commit actions on yourself!" << endl;
            return;
        }
        cout << p->getUsername() << " has " << p->getCoins() << " coins." << endl;
    }

    void Spy::blockArrest(Player* p) {
        if(!p->isAlive()) return;
        if (username == p->getUsername()) {
            cout << "Cannot commit actions on yourself!" << endl;
            return;
        }
        p->setCantDoArrest(true);
    }
}