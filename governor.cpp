// noashalom5@gmail.com
#include <iostream>
#include <stdexcept> 
#include <string>
using namespace std;

namespace coup {
    bool Governor::wantsToUndoTax() const {
        string answer;
        cout << "Governor " << getUsername() << ", do you want to undo the tax? (yes/no): ";
        cin >> answer; 
        return answer == "yes";
    }
}