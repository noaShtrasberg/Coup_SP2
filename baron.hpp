// noashalom5@gmail.com

#ifndef BARON_HPP
#define BARON_HPP

#include "player.hpp"

using namespace std;

namespace coup {
    
class Baron : public Player { 
    public:
        Baron(Game* game, const string& name)
            : Player(game, name) {
                type = "Baron";
            }
        virtual ~Baron() override {}
        
        /**
        * @brief Executes the Baron's special turn.
        *        Calls the invest action.
        */
        void specialTurn() override;

        /**
        * @brief Baron invests 3 coins (if they have at least 3) and gets 6 instead.
        * Ends the turn and updates last action.
        */
        void invest();
    };
}

#endif