// noashalom5@gmail.com
#ifndef SPY_HPP
#define SPY_HPP

#include <string>
using namespace std;

namespace coup {
    
class Spy : public Player { 
    public:
        Spy(const string& game, const string& name)
            : Player(game, name) {
                type = "Spy";
            }
        virtual ~Spy() override {}
        
        void specialTurn() override;
        void seeCoins(Player& p);
        void blockArrest(Player& p);
    };
}