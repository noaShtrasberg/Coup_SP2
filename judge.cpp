// noashalom5@gmail.com

#include "judge.hpp"
#include "player.hpp"
#include <iostream>

using namespace std;

namespace coup {
    bool Judge::wantsToUndoBribe() const {
        string answer;
        cout << "Judge " << getUsername() << ", do you want to undo the bribe? (yes/no): ";
        cin >> answer;
        return answer == "yes";
    }
}