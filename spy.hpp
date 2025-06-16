// noashalom5@gmail.com
#ifndef SPY_HPP
#define SPY_HPP

#include "player.hpp"

using namespace std;

namespace coup {
    
class Spy : public Player { 
    public:
        Spy(Game* game, const string& name)
            : Player(game, name) {
                type = "Spy";
            }
        virtual ~Spy() override {}
        
        /**
        * @brief Executes the Spy's special turn.
        *        Asks the player whether they want to:
        *        1. See another player's coin count.
        *        2. Block another player's ability to arrest.
        * 
        * Based on user input, performs the relevant action.
        */
        void specialTurn() override;

        /**
        * @brief Reveals the number of coins a target player has.
        * @param p Pointer to the target Player.
        */
        void seeCoins(Player* p);

        /**
        * @brief Prevents a target player from performing an arrest.
        * @param p Pointer to the target Player.
        */
        void blockArrest(Player* p);
    };
}

#endif