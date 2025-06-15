// noashalom5@gmail.com
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.hpp"
#include "game.hpp"
#include "player.hpp"
#include "baron.hpp"
#include "judge.hpp"
#include "governor.hpp"
#include "general.hpp"
#include "spy.hpp"
#include "merchant.hpp"
#include <sstream>
#include <iostream>

using namespace std;
using namespace doctest;
using namespace coup;

TEST_CASE("Player basic getters/setters") {
    Game* game = new Game();
    
    game->addPlayer("Noa");
    game->addPlayer("Lior");
    Player* noa = game->findPlayerByUsername("Noa");
    Player* lior = game->findPlayerByUsername("Lior");
    CHECK(noa->getUsername() == "Noa");
    CHECK(lior->getUsername() == "Lior");
    CHECK(noa->getCoins() == 0);
    noa->setCoins(5);
    CHECK(noa->getCoins() == 5);
    delete game;
}

TEST_CASE("MyTurn and SkipTurn") {
    Game* game = new Game();
    
    game->addPlayer("Noa");
    game->addPlayer("Lior");
    Player* noa = game->findPlayerByUsername("Noa");
    Player* lior = game->findPlayerByUsername("Lior");

    CHECK(game->currentPlayer()->getUsername() == "Noa");
    CHECK(noa->myTurn() == true);
    CHECK(lior->myTurn() == false);

    noa->skipTurn();
    CHECK(game->currentPlayer()->getUsername() == "Lior");
    CHECK(noa->myTurn() == false);
    CHECK(lior->myTurn() == true);

    lior->skipTurn();
    CHECK(game->currentPlayer()->getUsername() == "Noa");
    delete game;
}

TEST_CASE("TenCoins") {
    Game* game = new Game();

    game->addPlayer("Noa");
    game->addPlayer("Lior");
    Player* noa = game->findPlayerByUsername("Noa");
    Player* lior = game->findPlayerByUsername("Lior");

    noa->skipTurn();
    lior->setCoins(10);

    CHECK(lior->tenCoins() == true);
    lior->setCoins(8);
    CHECK(lior->tenCoins() == false);
    delete game;
}

TEST_CASE("IsAlive") {
    Game* game = new Game();

    game->addPlayer("Lior");
    Player* lior = game->findPlayerByUsername("Lior");

    CHECK(lior->isAlive() == true);
    game->removePlayer("Lior");
    CHECK(game->findPlayerByUsername("Lior") == nullptr); // THe player Lior is not alive
    delete game;
}

TEST_CASE("Gather") {
    Game* game = new Game();

    Player* judge = new Judge(game, "Adi"); // Not a Baron
    game->addPlayer(judge);
    Player* baron = new Baron(game, "Noa"); // Baron
    game->addPlayer(baron);
    Player* governor = new Governor(game, "Lior"); // Governor
    game->addPlayer(governor);

    judge->setCoins(4);
    baron->setCoins(5);
    governor->setCoins(6);

    // Judge under sanction
    game->setPlayerTurnIndex(0);
    judge->setUnderSanction(true);
    judge->gather(); // Do nothing
    CHECK(judge->getCoins() == 4);

    // Baron under sanction
    game->setPlayerTurnIndex(1);
    baron->setUnderSanction(true);
    baron->gather(); // coins++
    CHECK(baron->getCoins() == 6);

    // Regular gather
    game->setPlayerTurnIndex(2);
    governor->setUnderSanction(false);
    governor->gather(); // coins++
    CHECK(governor->getCoins() == 7);
    delete game;
}

TEST_CASE("Tax") {
    Game* game = new Game();

    Player* judge = new Judge(game, "Adi"); // Not a Baron
    game->addPlayer(judge);
    Player* baron = new Baron(game, "Noa"); // Baron
    game->addPlayer(baron);
    Player* governor = new Governor(game, "Lior"); // Governor
    game->addPlayer(governor);

    judge->setCoins(4);
    baron->setCoins(5);
    governor->setCoins(6);

    // Judge under sanction
    game->setPlayerTurnIndex(0);
    judge->setUnderSanction(true);
    judge->tax(); // Do nothing
    CHECK(judge->getCoins() == 4);

    // Baron under sanction
    game->setPlayerTurnIndex(1);
    baron->setUnderSanction(true);
    baron->tax(); // coins++
    CHECK(baron->getCoins() == 6);

    // Governor tax
    game->setPlayerTurnIndex(2);
    governor->tax(); // coins+=3
    CHECK(governor->getCoins() == 9);

    // Regular tax
    game->setPlayerTurnIndex(0);
    judge->setUnderSanction(false);
    judge->tax(); // coins+=2
    CHECK(judge->getCoins() == 6);
    delete game;
}

TEST_CASE("Bribe") {
    Game* game = new Game();

    Player* governor = new Governor(game, "Adi"); // Not a Baron
    game->addPlayer(governor);
    Player* baron = new Baron(game, "Noa"); // Baron
    game->addPlayer(baron);

    governor->setCoins(3);
    baron->setCoins(5);

    // Not enough coins to do bribe
    stringstream buffer;
    streambuf* oldCout = cout.rdbuf(buffer.rdbuf());  // replace cout to catch the output

    governor->bribe(); // Do nothing

    cout.rdbuf(oldCout);  // regular cout
    string expected = "Error: You don't have enough coins to performed bribe! pick another action.\n";
    CHECK(buffer.str() == expected);
    CHECK(governor->getCoins() == 3);

    // Regular bribe
    game->setPlayerTurnIndex(1); // Noa's turn
    baron->bribe(); // coins -=4
    CHECK(baron->getCoins() == 1);
    baron->gather();
    CHECK(game->getPlayerTurnIndex() == 1); // shoule be still Noa because the bribe
    delete game;
}

TEST_CASE("Arrest") {
    Game* game = new Game();


    Player* general = new General(game, "Noa"); // General
    game->addPlayer(general);
    Player* governor = new Governor(game, "Lior"); // Governor
    game->addPlayer(governor);
    Player* merchant = new Merchant(game, "Adi"); // Merchant
    game->addPlayer(merchant);
    Player* judge = new Judge(game, "Roni"); // Judge - the player doing the arrest
    game->addPlayer(judge);

    merchant->setCoins(6);
    general->setCoins(5);
    governor->setCoins(2);
    judge->setCoins(4);

    // Judge can't do arrest
    game->setPlayerTurnIndex(3);
    judge->setCantDoArrest(true);
    judge->arrest(governor); // Do nothing
    CHECK(judge->getCoins() == 4);
    CHECK(governor->getCoins() == 2);
    judge->setCantDoArrest(false);

    // Regular arrest and doubling
    game->setPlayerTurnIndex(3);
    judge->arrest(governor); // judge coins++, governor coins--
    CHECK(judge->getCoins() == 5);
    CHECK(governor->getCoins() == 1);
    game->setPlayerTurnIndex(3);
    judge->arrest(governor); // Again - do nothing
    CHECK(judge->getCoins() == 5);
    CHECK(governor->getCoins() == 1);
    CHECK(judge->getDidArrest() == "Lior");

    // Arrest on Merchant
    game->setPlayerTurnIndex(3);
    merchant->setCoins(6);
    judge->arrest(merchant); // judge coins the same, merchant coins-=2
    CHECK(judge->getCoins() == 5);
    CHECK(merchant->getCoins() == 4);
    CHECK(judge->getDidArrest()== "Adi");

    // Arrest on General
    game->setPlayerTurnIndex(3);
    judge->arrest(general); // Do nothing
    CHECK(judge->getCoins() == 5);
    CHECK(general->getCoins() == 5);

    // Arrest on someone without coins
    governor->setCoins(0);
    game->setPlayerTurnIndex(3);
    judge->arrest(governor); // Do nothing
    CHECK(judge->getCoins() == 5);
    CHECK(governor->getCoins() == 0);
    CHECK(judge->getDidArrest() == "Noa");
    delete game;
}

TEST_CASE("Sanction") {
    Game* game = new Game();

    Player* general = new General(game, "Noa"); // General
    game->addPlayer(general);
    Player* governor = new Governor(game, "Lior"); // Governor - the player doing the sanction
    game->addPlayer(governor);
    Player* judge = new Judge(game, "Roni"); // Judge 
    game->addPlayer(judge);

    general->setCoins(5);
    governor->setCoins(1);
    judge->setCoins(4);

    // Not enough coins for sanction
    game->setPlayerTurnIndex(1);
    governor->gather();
    game->setPlayerTurnIndex(1);
    governor->sanction(general); // Do nothing
    CHECK(governor->getCoins() == 2);
    CHECK(governor->getLastAction() == "gather");

    // Regular sanction on Judge
    governor->setCoins(7);
    governor->setCantDoArrest(true);
    CHECK(judge->isUnderSanction() == false);
    game->setPlayerTurnIndex(1);
    governor->sanction(judge); // governor coins-=4
    CHECK(governor->getCoins() == 3);
    CHECK(judge->isUnderSanction() == true);
    CHECK(governor->getCantDoArrest() == false); // After sanction
    delete game;
}

TEST_CASE("Coup") {
    Game* game = new Game();

    Player* merchant = new Merchant(game, "Adi"); // Merchant
    game->addPlayer(merchant);
    Player* governor = new Governor(game, "Lior"); // Governor
    game->addPlayer(governor);
    Player* judge = new Judge(game, "Roni"); // Judge - the player doing the coup
    game->addPlayer(judge);

    merchant->setCoins(6);
    governor->setCoins(2);
    judge->setCoins(4);
    cout << judge->getCoins() << endl;

    // Not enough coins for coup
    game->setPlayerTurnIndex(2);
    judge->tax();
    cout << judge->getCoins() << endl;
    CHECK(game->getPlayersList().size() == 3);
    judge->coup(merchant); // Do nothing
    CHECK(judge->getCoins() == 6); // 4 + tax
    CHECK(judge->getLastAction() == "tax");
    CHECK(game->getPlayersList().size() == 3);

    // Doing coup on the governor
    judge->setCoins(8);
    CHECK(game->getPlayersList().size() == 3);
    game->setPlayerTurnIndex(2);
    judge->setCantDoArrest(true);
    judge->coup(governor); 
    CHECK(judge->getCoins() == 1);
    CHECK(game->getPlayersList().size() == 2);
    CHECK(judge->getLastAction()== "coup");
    CHECK(judge->getCantDoArrest() == false);
    delete game;
}
