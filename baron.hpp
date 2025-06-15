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
        
        void specialTurn() override;
        void invest();
    };
}

#endif