// noashalom5@gmail.com

#include "game.hpp"
#include "player.hpp"
#include <iostream>
#include <string>
using namespace std;
using namespace coup;

int main() {
    Game game;
    int numPlayers;
    while (true) {
        cout << "Enter number of players (2-6): ";
        if (cin >> numPlayers && numPlayers >= 2 && numPlayers <= 6) {
            break;
        } else {
            cout << "Invalid input! Please enter another number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    cout << "Number of players: " << numPlayers << endl;

    cin.ignore();

    for (int i = 0; i < numPlayers; i++) {
        string name;
        cout << "Enter name for player " << (i + 1) << ": ";
        getline(cin, name);
        game.addPlayer(name);
    }

    cout << "Game started!" << endl;

    while (game.getPlayersList().size() > 1) {
        Player* current = game.currentPlayer();
        cout << "\nIt's " << current->getUsername() << "'s turn. Coins: " << current->getCoins() << endl;
        if(current->getRole()!="Baron" && current->getRole()!="Spy")
            cout << "Choose action: [1] Gather [2] Tax [3] Bribe [4] Arrest [5] Sanction [6] Coup [7] Skip\n";
        else
            cout << "Choose action: [1] Gather [2] Tax [3] Bribe [4] Arrest [5] Sanction [6] Coup [7] Skip [8] Special turn for Spy/Baron\n";
        int choice;
        cin >> choice;

        if (choice == 1) {
            current->gather();
        } else if (choice == 2) {
            current->tax();
        } else if (choice == 3) {
            current->bribe();
        } else if (choice == 4) {
            cout << "Enter player name to arrest: ";
            string targetName;
            cin >> targetName;
            Player* target = game.findPlayerByUsername(targetName);
            current->arrest(target);
        } else if (choice == 5) {
            cout << "Enter player name to sanction: ";
            string targetName;
            cin >> targetName;
            Player* target = game.findPlayerByUsername(targetName);
            current->sanction(target);
        } else if (choice == 6) {
            cout << "Enter player name to coup: ";
            string targetName;
            cin >> targetName;
            Player* target = game.findPlayerByUsername(targetName);
            current->coup(target);
        } else if (choice == 7) {
            current->skipTurn();
        } else if (choice == 8) {
            if(current->getRole() == "Baron") {
                cout << "Baron is investing coins..." << endl;
                current->specialTurn();
            }
            else if(current->getRole() == "Spy")
                current->specialTurn();
            else
                cout << "Invalid choice. Try again." << endl;
        } else {
            cout << "Invalid choice. Try again." << endl;
        }
    }

    game.winner();
    return 0;
}