// noashalom5@gmail.com
#include "game.hpp"
#include "player.hpp"
#include <iostream>

using namespace std;

namespace coup {
    Game::Game() : playerTurnIndex(0), gameStarted(false), numPlayers(0) {}

    Game::~Game() {
        for (Player* player : playersList) {
            delete player;
        }
        playersList.clear(); 
        gameUsernames.clear();
    }

    // Add new player to the game
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

        int num = rand() % 6;
        Player* newPlayer = nullptr;

        if (num == 0) {
            newPlayer = new Governor(username);
        } else if (num == 1) {
            newPlayer = new Spy(username);
        } else if (num == 2) {
            newPlayer = new Baron(username);
        } else if (num == 3) {
            newPlayer = new General(username);
        } else if (num == 4) {
            newPlayer = new Judge(username);
        } else if (num == 5) {
            newPlayer = new Merchant(username);
        }
        playersList.push_back(newPlayer);
        gameUsernames.insert(username);
        numPlayers++;
        cout << "Player \"" << username << "\" has joined the game" << endl;
    }

    void Game::addPlayer(Player* player) {
        playersList.push_back(player);
    }

    // Remove a player from the game
    void Game::removePlayer(const string& username) {
        Player* p = findPlayerByUsername(username);
        if (p == nullptr) {
            return;
        }
        for (auto player=playersList.begin() ; player!=playersList.end() ; player++) {
            if (*player == p) {
                int index = distance(playersList.begin(), player);
                delete *player;
                playersList.erase(player);
                numPlayers--;
                if (index < playerTurnIndex) // save the turn index
                    playerTurnIndex--;
                break;
            }
        }
    }

    // Get a reference to the list of players
    const vector<Player*>& Game::getPlayersList() const {
        return playersList;
    }

    Player* Game::findPlayerByUsername(const string& username) const {
        for (Player* player : playersList) {
            if (player->getUsername() == username) {
                return player;
            }
        }
        return nullptr;
    }

    // Get the current player turn index
    int Game::getPlayerTurnIndex() const {
        return playerTurnIndex;
    }

    void Game::setPlayerTurnIndex(int n) {
        playerTurnIndex =  n;
    }

    bool Game::getGameStarted() const {
        return gameStarted;
    }

    void Game::setGameStarted(bool value) {
        gameStarted = value;
    }

    bool Game::thereIs(string type) const { // judge - bribe, general - coup
        bool exist = false;
        for (const auto& player : playersList) {
            if(player->getRole() == type) {
                exist = true;
                break;
            }
        }
        return exist;
    } 

    bool Game::undoBribe() const {
        bool undo = false;
        Player* p = currentPlayer();
            for (const auto& player : playersList) {
                if(player->getRole()=="Judge" && player->getUsername()!=p->getUsername()){
                    Judge* judge = static_cast<Judge*>(player);
                    if(judge->wantsToUndoBribe()) {
                        undo = true;
                        break;
                    }   
                }
            }
            return undo;
    }

    bool Game::undoCoup() const {
        bool undo = false;
        Player* p = currentPlayer();
            for (const auto& player : playersList) {
                if(player->getRole()=="General" && player->getCoins()>=5 && player->getUsername()!=p->getUsername()){
                    General* general = static_cast<General*>(player);
                    if(general->wantsToUndoCoup()) {
                        undo = true;
                        break;
                    } 
                }
            }
            return undo;
    }

    bool Game::undoTax() const {
        bool undo = false;
        Player* p = currentPlayer();
            for (const auto& player : playersList) {
                if(player->getRole()=="Governor" && player->getUsername()!=p->getUsername()){
                    Governor* governor = static_cast<Governor*>(player);
                    if(governor->wantsToUndoTax()) {
                        undo = true;
                        break;
                    } 
                }
            }
            return undo;
    }

    // Get the current player based on the turn index
    Player* Game::currentPlayer() const {
        if (playersList.empty()) {
            cout << "Error: No players in the game." << endl;
            return nullptr;
        }
        return playersList[playerTurnIndex];
    }

    string Game::turn() const {
        Player* p = currentPlayer();
        string turnName = p->getUsername();
        return turnName;
    }

    void Game::moveTurn() {
        if(currentPlayer()->getLastAction() == "bribe") {
            cout << currentPlayer()->getUsername() << " has one more turn, he performed bribe." << endl;
        }
        else {
            int newIndex = (getPlayerTurnIndex() + 1) % (getPlayersList().size()); // For new cycle - %size=0
            setPlayerTurnIndex(newIndex);
            // Turn of merchant with 3+ coins -> he gets one more
            Player* p = currentPlayer();
            if(p->getRole() == "Merchant" && p->getCoins()>=3) {
                p->setCoins(p->getCoins()+1);
            }
        }
    }

    // Get the names of all the players currently in the game
    void Game::players() const {
        for (const auto& player : playersList) {
            cout << player->getUsername() << endl;
        }
    }

    // Return the winner if there is one, otherwise print an error
    void Game::winner() const {
        if (playersList.size() != 1) {
            cout << "Error: The game is not finished yet." << endl;
            return;
        }
        cout << "The game is over! The winner is " << playersList.front()->getUsername() << "." << endl;
    }
}