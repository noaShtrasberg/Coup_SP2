// noashalom5@gmail.com

#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include "player.hpp"

using namespace std;

namespace coup {

class Game {
private:
    vector<Player*> playersList;
    set<string> gameUsernames;
    int numPlayers;
    int playerTurnIndex;
    bool gameStarted;

public:
    Game();
    ~Game();
    
    void addPlayer(const string& username);
    void removePlayer(Player* player);
    void addPlayer(Player* player);

    const vector<Player*>& getPlayersList() const;
    Player* findPlayerByUsername(const string& username) const;
    int getPlayerTurnIndex() const;
    void setPlayerTurnIndex(int n);
    bool getGameStarted() const;
    void setGameStarted(bool value);

    bool thereIs(string type) const; // judge - bribe, general - coup
    bool undoBribe() const;
    bool undoCoup() const;
    bool undoTax() const;

    Player* currentPlayer() const;
    string turn() const;
    /**
    * @brief Advance the turn to the next player.
    * If the last action was "bribe", the current player gets an extra turn.
    */
    void moveTurn();
    void players() const;
    void winner() const;

}
}
