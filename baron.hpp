// noashalom5@gmail.com

#ifndef BARON_HPP
#define BARON_HPP

#include <string>
using namespace std;

namespace coup {
    
class Baron : public Player { 
    public:
        Baron(const string& game, const string& name)
            : Player(game, name) {
                type = "Baron";
            }
        virtual ~Baron() override {}
        
        void specialTurn() override;
        void invest() const;
    };
}