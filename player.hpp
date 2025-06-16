// noashalom5@gmail.com
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <string>
using namespace std;

namespace coup {
class Game; 
    
class Player { 
protected:
    Game* game;
    string username;
    string lastAction;
    string didArrest;
    string type;
    int coins;
    bool underSanction;
    bool cantDoArrest;

public:
    Player(Game* game, const string& username)
    : game(game), username(username), coins(0), underSanction(false), cantDoArrest(false) {}

    virtual ~Player() {}

    /**
    * @brief Get the type of the player.
    * @return The player's type.
    */
    string getRole() const {return type;}

    /**
    * @brief Get the username of the player.
    * @return The player's username.
    */
    string getUsername() const {return username;}

    /**
    * @brief Get the current amount of coins the player has.
    * @return The number of coins.
    */
    int getCoins() const {return coins;}

    /**
    * @brief Set the player's coin count.
    * @param c The new amount of coins.
    */
    void setCoins(int c) {coins = c;}

    /**
    * @brief Get the last action the player performed.
    * @return A string representing the player's last action.
    */
    string getLastAction() const {return lastAction;}

    /**
    * @brief Set whether the player is unable to perform an arrest.
    * @param value True if the player cannot arrest, false otherwise.
    */
    void setCantDoArrest(bool value) {cantDoArrest = value;}

    /**
    * @brief Set whether the player is currently under sanction.
    * @param value True if the player is under sanction, false otherwise.
    */
    void setUnderSanction(bool value) {underSanction = value;}

    /**
    * @brief Get the name of the player the current player arrested.
    * @return A string with the name of the arrested player, or an empty string if no arrest was made.
    */
    string getDidArrest() const { return didArrest;}

    /**
    * @brief Check if the player is currently under sanction.
    * @return True if the player is under sanction, false otherwise.
    */
    bool isUnderSanction() const { return underSanction;}

    /**
    * @brief Check if the player is currently restricted from performing an arrest.
    * @return True if the player cannot arrest, false otherwise.
    */
    bool getCantDoArrest() const { return cantDoArrest;}

    /**
    * @brief Check if it is this player's turn.
    * Prints an error if not.
    * @return True if it's this player's turn; otherwise, false.
    */
    bool myTurn() const;

    /**
    * @brief Check if the player has 10 or more coins.
    * If so, prints an error because the player must perform a coup.
    * @return True if player has >= 10 coins; otherwise, false.
    */
    bool tenCoins() const;

    /**
    * @brief Check if the player is still in the game.
    * @return True if the player is alive; otherwise, false.
    */
    bool isAlive() const;

    /**
    * @brief Skip the player's turn if possible.
    * Also clears any arrest restriction for the next turn.
    */
    void skipTurn();
    
    /**
    * @brief Perform the "gather" action to gain 1 coin.
    * Fails if under sanction (unless player is Baron).
    */
    void gather();

    /**
    * @brief Perform the "tax" action.
    * Player may receive 2 or 3 coins depending on their role and game state.
    * Action may be blocked by a Governor.
    */
    void tax();

    /**
    * @brief Attempts to perform a bribe action. 
    * Reduces 4 coins from the player. If a Judge is in the game, the action might be blocked.
    * If blocked, the action has no effect.
    * Updates the last action and disables the arrest restriction if needed.
    */
    void bribe();

    /**
    * @brief Attempts to arrest another player.
    * @param p The target player to arrest.
    * If the target is a Merchant with at least 2 coins, 2 coins are removed not to the player.
    * If the target is General, nothing happends.
    * Otherwise, 1 coin is transferred from the target to the player.
    * Cannot arrest the same player twice in a row or if under restriction.
    * Advances the turn and updates the last action.
    */
    void arrest(Player* p);

    /**
    * @brief Attempts to place a sanction on another player.
    * @param p The player to be sanctioned.
    * Costs 3 coins (4 if the target is a Judge). Sanction prevents the player from performing tax and gather.
    * Advances the turn and updates the last action.
    */
    void sanction(Player* p);

    /**
    * @brief Performs a coup against another player.
    * @param p The target player to eliminate from the game.
    * Costs 7 coins. The action may be blocked by a General (with 5 coins).
    * If not blocked, the target player is eliminated and removed from the game.
    * Updates the last action and disables the arrest restriction if needed.
    */
    void coup(Player* p);
    
    /**
    * @brief Performs a special action for specific players.
    * For Baron - invest. pays 3 coins and gets 6 instead. Count as a turn.
    * For Spy - two option: 1. See coins of another player.
    * 2. Block arrest of another player. Not count as a turn.
    */
    virtual void specialTurn() {} 
};
}

#endif