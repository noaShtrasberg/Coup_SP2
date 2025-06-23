// noashalom5@gmail.com

#include "general.hpp"
#include "player.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

namespace coup {

    bool General::wantsToUndoCoup() const {
        string answer;
        cout << "General " << getUsername() << ", do you want to undo the coup? (yes/no): ";
        cin >> answer;
        return answer == "yes";
    }
}