// noashalom5@gmail.com
#include <iostream>
#include <stdexcept> 
#include <string>
using namespace std;

namespace coup {
    bool General::wantsToUndoCoup() const {
        string answer;
        cout << "General " << getUsername() << ", do you want to undo the coup? (yes/no): ";
        cin >> answer;
        return answer == "yes";
    }
}