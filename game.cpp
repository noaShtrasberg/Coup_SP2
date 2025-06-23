#include "game.hpp"
#include "player.hpp"
#include "governor.hpp"
#include "spy.hpp"
#include "baron.hpp"
#include "general.hpp"
#include "judge.hpp"
#include "merchant.hpp"
#include "logger.hpp"

using namespace std;

namespace coup {
    Game::Game() : numPlayers(0), playerTurnIndex(0), gameStarted(false) {}

    Game::~Game() {
        for (Player* player : playersList) {
            delete player;
        }
        playersList.clear(); 
        gameUsernames.clear();
    }

    void Game::addPlayer(const string& username) {
        if (gameStarted) {
            cout << "The game has already started. You cannot join." << endl;
            return;
        }
        if (numPlayers == 6) {
            cout << "The game is full. Please join another game." << endl;
            return;
        }
        if (gameUsernames.count(username)) {
            cout << "Username is already taken. Please choose another one." << endl;
            return;
        }

        srand(time(0));
        int num = rand() % 6;
        Player* newPlayer = nullptr;

        if (num == 0) newPlayer = new Governor(this, username);
        else if (num == 1) newPlayer = new Spy(this, username);
        else if (num == 2) newPlayer = new Baron(this, username);
        else if (num == 3) newPlayer = new General(this, username);
        else if (num == 4) newPlayer = new Judge(this, username);
        else newPlayer = new Merchant(this, username);

        playersList.push_back(newPlayer);
        gameUsernames.insert(username);
        numPlayers++;
        cout << "Player \"" + username + "\" has joined the game" << endl;
    }

    void Game::addPlayer(Player* player) {
        playersList.push_back(player);
    }

    void Game::removePlayer(const string& username) {
        Player* p = findPlayerByUsername(username);
        if (!p) return;
        for (auto player = playersList.begin(); player != playersList.end(); ++player) {
            if (*player == p) {
                int index = distance(playersList.begin(), player);
                delete *player;
                playersList.erase(player);
                numPlayers--;
                if (index < playerTurnIndex) playerTurnIndex--;
                break;
            }
        }
    }

    const vector<Player*>& Game::getPlayersList() const {
        return playersList;
    }

    Player* Game::findPlayerByUsername(const string& username) const {
        for (Player* player : playersList) {
            if (player->getUsername() == username) return player;
        }
        return nullptr;
    }

    bool Game::thereIs(string type) const {
        for (const auto& player : playersList) {
            if (player->getRole() == type) return true;
        }
        return false;
    }

    bool Game::undoBribe() const {
        Player* p = currentPlayer();
        for (const auto& player : playersList) {
            if (player->getRole() == "Judge" && player->getUsername() != p->getUsername()) {
                auto* judge = static_cast<Judge*>(player);
                if (judge->wantsToUndoBribe()) return true;
            }
        }
        return false;
    }

    bool Game::undoCoup() const {
        Player* p = currentPlayer();
        for (const auto& player : playersList) {
            if (player->getRole() == "General" && player->getCoins() >= 5 && player->getUsername() != p->getUsername()) {
                auto* general = static_cast<General*>(player);
                if (general->wantsToUndoCoup()) {
                    general->setCoins(general->getCoins()-5);
                    return true;
                }
            }
        }
        return false;
    }

    bool Game::undoTax() const {
        Player* p = currentPlayer();
        for (const auto& player : playersList) {
            if (player->getRole() == "Governor" && player->getUsername() != p->getUsername()) {
                auto* governor = static_cast<Governor*>(player);
                if (governor->wantsToUndoTax()) return true;
            }
        }
        return false;
    }

    Player* Game::currentPlayer() const {
        if (playersList.empty()) {
            cout << "Error: No players in the game." << endl;
            return nullptr;
        }
        return playersList[playerTurnIndex];
    }

    string Game::turn() const {
        return currentPlayer()->getUsername();
    }

    void Game::moveTurn() {
        if (currentPlayer()->getLastAction() == "bribe") {
            cout << currentPlayer()->getUsername() + " has two more turns, this performed bribe." << endl;
        } else if (currentPlayer()->getActionBribe() == 1) {
            cout << currentPlayer()->getUsername() + " has one more turn, this performed bribe." << endl;
        } else {
            int newIndex = (getPlayerTurnIndex() + 1) % getPlayersList().size();
            setPlayerTurnIndex(newIndex);
            Player* p = currentPlayer();
            if (p->getRole() == "Merchant" && p->getCoins() >= 3) {
                p->setCoins(p->getCoins() + 1);
            }
        }
    }

    void Game::players() const {
        for (const auto& player : playersList) {
            cout << player->getUsername() << endl;
        }
    }

    void Game::winner() const {
        if (playersList.size() != 1) {
            cout << "Error: The game is not finished yet." << endl;
            return;
        }
        cout << "The game is over! The winner is " + playersList.front()->getUsername() + "." << endl;
    }
}
