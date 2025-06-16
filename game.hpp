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
    
    /**
    * @brief Adds a new player to the game with the given username.
    * Assigns a random role (Governor, Spy, etc.).
    * @param username The username of the player to add.
    */
    void addPlayer(const string& username);

    /**
    * @brief Adds an already created player instance to the game.
    * @param player Pointer to the Player object to add.
    */
    void addPlayer(Player* player);

    /**
    * @brief Removes a player from the game by username.
    * Also adjusts the turn index if needed.
    * @param username The username of the player to remove.
    */
    void removePlayer(const string& username);
    
    /**
    * @brief Retrieves the list of players currently in the game.
    * @return A constant reference to the vector of Player pointers.
    */
    const vector<Player*>& getPlayersList() const;

    /**
    * @brief Finds a player object in the game by username.
    * @param username The username to search for.
    * @return A pointer to the Player if found, nullptr otherwise.
    */
    Player* findPlayerByUsername(const string& username) const;

    /**
    * @brief Returns the index of the player whose turn it currently is.
    * @return An integer representing the turn index.
    */
    int getPlayerTurnIndex() const;

    /**
    * @brief Sets the current player turn index.
    * @param n The new index to set.
    */
    void setPlayerTurnIndex(int n);

    /**
    * @brief Checks if the game has started.
    * @return True if the game has started, false otherwise.
    */
    bool getGameStarted() const;

    /**
    * @brief Sets the game started status.
    * @param value True to indicate the game has started.
    */
    void setGameStarted(bool value);

    /**
    * @brief Checks whether a player with a specific role exists in the game.
    * @param type The role to check for (e.g., "Judge", "General").
    * @return True if such a player exists, false otherwise.
    */
    bool thereIs(string type) const; // judge - bribe, general - coup

    /**
    * @brief Checks whether a bribe action can be undone by a Judge.
    * @return True if a Judge wants to undo the bribe, false otherwise.
    */
    bool undoBribe() const;

    /**
    * @brief Checks whether a coup action can be undone by a General.
    * @return True if a General wants to undo the coup, false otherwise.
    */
    bool undoCoup() const;

    /**
    * @brief Checks whether a tax action can be undone by a Governor.
    * @return True if a Governor wants to undo the tax, false otherwise.
    */
    bool undoTax() const;

    /**
    * @brief Returns the player whose turn it currently is.
    * @return A pointer to the current Player.
    */
    Player* currentPlayer() const;

    /**
    * @brief Returns the username of the player whose turn it is.
    * @return A string with the current player's username.
    */
    string turn() const;

    /**
    * @brief Advances the turn to the next player.
    * Special cases handled for bribe (extra turn) and merchant (extra coin).
    */
    void moveTurn();

    /**
    * @brief Prints the usernames of all the players currently in the game.
    */
    void players() const;

    /**
    * @brief Announces the winner if only one player remains.
    * Otherwise, prints an error message.
    */
    void winner() const;

};
}

#endif
