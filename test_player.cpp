// noashalom5@gmail.com
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "game.hpp"
#include "doctest.hpp"
#include "player.hpp"
#include <sstream>
#include <iostream>

using namespace std;
using namespace doctest;
using namespace coup;

TEST_CASE("Player basic getters/setters") {
    Game game;
    
    game.addPlayer("Noa");
    game.addPlayer("Lior");
    Player* noa = game.findPlayerByUsername("Noa");
    Player* lior = game.findPlayerByUsername("Lior");
    CHECK(noa->getUsername() == "Noa");
    CHECK(lior->getUsername() == "Lior");
    CHECK(noa->getCoins() == 0);
    noa->setCoins(5);
    CHECK(noa->getCoins() == 5);
}

TEST_CASE("MyTurn and SkipTurn") {
    Game game;
    
    game.addPlayer("Noa");
    game.addPlayer("Lior");
    Player* noa = game.findPlayerByUsername("Noa");
    Player* lior = game.findPlayerByUsername("Lior");

    CHECK(game.currentPlayer().getUsername() == "Noa");
    CHECK(noa->myTurn() == true);
    CHECK(lior->myTurn() == false);

    noa->skipTurn();
    CHECK(game.currentPlayer().getUsername() == "Lior");
    CHECK(noa->myTurn() == false);
    CHECK(lior->myTurn() == true);

    lior->skipTurn();
    CHECK(game.currentPlayer().getUsername() == "Noa");
}

TEST_CASE("TenCoins") {
    Game game;

    game.addPlayer("Noa");
    game.addPlayer("Lior");
    Player* noa = game.findPlayerByUsername("Noa");
    Player* lior = game.findPlayerByUsername("Lior");

    noa->skipTurn();
    lior->setCoins(10);

    CHECK(lior->tenCoins() == true);
    lior->setCoins(8);
    CHECK(lior->tenCoins() == false);
}

TEST_CASE("IsAlive") {
    Game game;

    game.addPlayer("Noa");
    game.addPlayer("Lior");
    Player* noa = game.findPlayerByUsername("Noa");
    Player* lior = game.findPlayerByUsername("Lior");

    CHECK(lior->isAlive() == true);
    game.removePlayer("Lior");
    CHECK(lior->isAlive() == false);
}

TEST_CASE("Gather") {
    Game game;

    Player* judge = new Judge("Adi"); // Not a Baron
    game.addPlayer(judge);
    Player* baron = new Baron("Noa"); // Baron
    game.addPlayer(baron);
    Player* governor = new Governor("Lior"); // Governor
    game.addPlayer(governor);

    judge->setCoins(4);
    baron->setCoins(5);
    governor->setCoins(6);

    // Judge under sanction
    judge->underSanction = true;
    judge->gather(); // Do nothing
    CHECK(judge->getCoins() == 4);

    // Baron under sanction
    baron->underSanction = true;
    baron->gather(); // coins++
    CHECK(baron->getCoins() == 6);

    // Regular gather
    governor->underSanction = false;
    governor->gather(); // coins++
    CHECK(governor->getCoins() == 7);
}

TEST_CASE("Tax") {
    Game game;

    Player* judge = new Judge("Adi"); // Not a Baron
    game.addPlayer(judge);
    Player* baron = new Baron("Noa"); // Baron
    game.addPlayer(baron);
    Player* governor = new Governor("Lior"); // Governor
    game.addPlayer(governor);

    judge->setCoins(4);
    baron->setCoins(5);
    governor->setCoins(6);

    // Judge under sanction
    judge->underSanction = true;
    judge->tax(); // Do nothing
    CHECK(judge->getCoins() == 4);

    // Baron under sanction
    baron->underSanction = true;
    baron->tax(); // coins++
    CHECK(baron->getCoins() == 6);

    // Governor tax
    governor->tax(); // coins+=3
    CHECK(governor->getCoins() == 9);

    // Regular tax
    judge->underSanction = false;
    judge->tax(); // coins+=2
    CHECK(judge->getCoins() == 6);
}

TEST_CASE("Bribe") {
    Game game;

    Player* governor = new Governor("Adi"); // Not a Baron
    game.addPlayer(governor);
    Player* baron = new Baron("Noa"); // Baron
    game.addPlayer(baron);

    governor->setCoins(3);
    baron->setCoins(5);

    // Not enough coins to do bribe
    stringstream buffer;
    streambuf* oldCout = cout.rdbuf(buffer.rdbuf());  // replace cout to catch the output

    governor->bribe(); // Do nothing

    cout.rdbuf(oldCout);  // regular cout
    string expected = "Error: You don't have enough coins to performed bribe! pick another action.";
    CHECK(buffer.str() == expected);
    CHECK(governor->getCoins() == 3);

    // Regular bribe
    game->setPlayerTurnIndex(1); // Noa's turn
    baron->bribe(); // coins -=4
    CHECK(governor->getCoins() == 1);
    baron->tax();
    CHECK(game->getPlayerTurnIndex() == 1) // shoule be still Noa because the bribe
}

TEST_CASE("Arrest") {
    Game game;

    Player* merchant = new Merchant("Adi"); // Merchant
    game.addPlayer(merchant);
    Player* general = new General("Noa"); // General
    game.addPlayer(general);
    Player* governor = new Governor("Lior"); // Governor
    game.addPlayer(governor);
    Player* judge = new Judge("Roni"); // Judge - the player doing the arrest
    game.addPlayer(judge);

    merchant->setCoins(6);
    general->setCoins(5);
    governor->setCoins(2);
    judge->setCoins(4);

    // Judge can't do arrest
    judge->cantDoArrest = true;
    judge->arrest(governor); // Do nothing
    CHECK(judge->getCoins() == 4);
    CHECK(governor->getCoins() == 2);
    CHECK(judge->cantDoArrest == true);
    judge->cantDoArrest = false;

    // Regular arrest and doubling
    judge->arrest(governor); // judge coins++, governor coins--
    CHECK(judge->getCoins() == 5);
    CHECK(governor->getCoins() == 1);
    judge->arrest(governor); // Again - do nothing
    CHECK(judge->getCoins() == 5);
    CHECK(governor->getCoins() == 1);
    CHECK(judge->didArrest == "Lior");

    // Arrest on Merchant
    judge->arrest(merchant); // judge coins the same, merchant coins-=2
    CHECK(judge->getCoins() == 5);
    CHECK(merchant->getCoins() == 4);
    CHECK(judge->didArrest == "Adi");

    // Arrest on General
    judge->arrest(general); // Do nothing
    CHECK(judge->getCoins() == 5);
    CHECK(general->getCoins() == 5);

    // Arrest on someone without coins
    governor->setCoins(0);
    judge->arrest(governor); // Do nothing
    CHECK(judge->getCoins() == 5);
    CHECK(governor->getCoins() == 0);
    CHECK(judge->didArrest == "Lior");
}

TEST_CASE("Sanction") {
    Game game;

    Player* general = new General("Noa"); // General
    game.addPlayer(general);
    Player* governor = new Governor("Lior"); // Governor - the player doing the sanction
    game.addPlayer(governor);
    Player* judge = new Judge("Roni"); // Judge 
    game.addPlayer(judge);

    general->setCoins(5);
    governor->setCoins(2);
    judge->setCoins(4);

    // Not enough coins for sanction
    governor->sanction(general); // Do nothing
    CHECK(governor->getCoins() == 2);
    CHECK(governor->lastAction != "sanction");

    // Regular sanction on Judge
    governor->setCoins(7);
    governor->cantDoArrest = true;
    CHECK(judge->underSanction == false);
    governor->sanction(judge); // governor coins-=4
    CHECK(governor->getCoins() == 3);
    CHECK(judge->underSanction == true);
    CHECK(governor->cantDoArrest == false); // After sanction
}

TEST_CASE("Coup") {
    Game game;

    Player* merchant = new Merchant("Adi"); // Merchant
    game.addPlayer(merchant);
    Player* governor = new Governor("Lior"); // Governor
    game.addPlayer(governor);
    Player* judge = new Judge("Roni"); // Judge - the player doing the coup
    game.addPlayer(judge);

    merchant->setCoins(6);
    governor->setCoins(2);
    judge->setCoins(4);

    // Not enough coins for sanction
    judge->tax();
    judge->cantDoArrest = true;
    CHECK(game->getPlayersList().size() == 3);
    judge->coup(merchant); // Do nothing
    CHECK(judge->getCoins() == 5); // 4 + tax
    CHECK(judge->lastAction == "tax");
    CHECK(game->getPlayersList().size() == 3);
    CHECK(judge->cantDoArrest == false);

    // Doing coup on the governor
    judge->setCoins(8);
    CHECK(game->getPlayersList().size() == 3);
    judge->coup(governor); 
    CHECK(judge->getCoins() == 1);
    CHECK(game->getPlayersList().size() == 2);
    CHECK(judge->lastAction == "coup");
}
