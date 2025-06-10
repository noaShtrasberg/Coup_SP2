// noashalom5@gmail.com

#ifndef GENERAL_HPP
#define GENERAL_HPP

#include <string>
using namespace std;

namespace coup {
    
class General : public Player { 
    public:
        General(const string& game, const string& name)
            : Player(game, name) {
                type = "General";
            }
        virtual ~General() override {}
        
        bool wantsToUndoCoup() const;
    };
}