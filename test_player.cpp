// noashalom5@gmail.com
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Game.hpp"
#include "doctest.hpp"
#include "Player.hpp"
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

