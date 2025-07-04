// noashalom5@gmail.com
#ifndef GOVERNOR_HPP
#define GOVERNOR_HPP

#include "player.hpp"

using namespace std;

namespace coup {
    
class Governor : public Player { 
    public:
        Governor(Game* game, const string& name)
            : Player(game, name) {
                type = "Governor";
            }
        virtual ~Governor() override {}
        
        bool wantsToUndoTax() const;
    };
}

#endif