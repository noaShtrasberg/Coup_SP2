// noashalom5@gmail.com
#ifndef MERCHANT_HPP
#define MERCHANT_HPP

#include <string>
using namespace std;

namespace coup {
    
class Merchant : public Player { 
    public:
        Merchant(const string& game, const string& name)
            : Player(game, name) {
                type = "Merchant";
            }
        virtual ~Merchant() override {}
    };
}