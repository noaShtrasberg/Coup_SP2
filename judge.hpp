// noashalom5@gmail.com

#ifndef JUDGE_HPP
#define JUDGE_HPP

#include "player.hpp"

using namespace std;

namespace coup {
    
class Judge : public Player { 
    public:
        Judge(Game* game, const string& name)
            : Player(game, name) {
                type = "Judge";
            }
        virtual ~Judge() override {}
        
        bool wantsToUndoBribe() const;
    };
}

#endif