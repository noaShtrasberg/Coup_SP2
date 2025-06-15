// noashalom5@gmail.com
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "game.hpp"
#include "doctest.hpp"
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

TEST_CASE("AddPlayer") {
    Game* game = new Game();

    SUBCASE("Add new player successfully") {
        game->addPlayer("Noa");
        CHECK(game->getPlayersList().size() == 1);
        CHECK(game->findPlayerByUsername("Noa") != nullptr);
    }

    SUBCASE("Add player with duplicate username") {
        game->addPlayer("Roi");
        size_t beforeSize = game->getPlayersList().size(); // == 1
        game->addPlayer("Roi"); 
        CHECK(game->getPlayersList().size() == beforeSize);
    }

    SUBCASE("Add player after game started") {
        game->addPlayer("Oz");
        game->setGameStarted(true);
        size_t beforeSize = game->getPlayersList().size(); // == 1
        game->addPlayer("Roi");
        CHECK(game->getPlayersList().size() == beforeSize);
    }

    SUBCASE("Add more than 6 players") {
        for (int i = 0; i < 6; ++i) {
            game->addPlayer("Player" + to_string(i));
        }
        CHECK(game->getPlayersList().size() == 6);
        size_t beforeSize = game->getPlayersList().size();
        // Try to add player number 7
        game->addPlayer("Noa");
        CHECK(game->getPlayersList().size() == beforeSize);
    }
    delete game;
}

TEST_CASE("RemovePlayer") {
    Game* game = new Game();

    game->addPlayer("Noa");
    game->addPlayer("Roi");
    game->addPlayer("Adi");

    CHECK(game->getPlayersList().size() == 3);
    CHECK(game->findPlayerByUsername("Adi") != nullptr);

    game->removePlayer("Adi");
    CHECK(game->getPlayersList().size() == 2);
    CHECK(game->findPlayerByUsername("Adi") == nullptr);
    CHECK(game->findPlayerByUsername("Noa") != nullptr);
    CHECK(game->findPlayerByUsername("Roi") != nullptr);

    // Try to remove by username that not exist
    game->removePlayer("NotExist");
    CHECK(game->getPlayersList().size() == 2);
    delete game;
}

TEST_CASE("FindPlayerByUsername") {
    Game* game = new Game();
    game->addPlayer("Adi");
    game->addPlayer("Oz");

    SUBCASE("Existing player") {
        Player* adi = game->findPlayerByUsername("Adi");
        REQUIRE(adi != nullptr);
        CHECK(adi->getUsername() == "Adi");

        Player* oz = game->findPlayerByUsername("Oz");
        REQUIRE(oz != nullptr);
        CHECK(oz->getUsername() == "Oz");
    }

    SUBCASE("Non-existing player") {
        Player* notExist = game->findPlayerByUsername("Barbi");
        CHECK(notExist == nullptr);
    }
    delete game;
}

TEST_CASE("ThereIs") {
    Game* game = new Game();
    CHECK(game->thereIs("Judge") == false);
    CHECK(game->thereIs("General") == false);

    Player* judge = new Judge(game, "Noa");
    game->addPlayer(judge);
    CHECK(game->thereIs("Judge") == true);
    CHECK(game->thereIs("General") == false);

    Player* general = new General(game, "Lior");
    game->addPlayer(general);
    CHECK(game->thereIs("Judge") == true);
    CHECK(game->thereIs("General") == true);

    game->removePlayer("Noa");
    CHECK(game->thereIs("Judge") == false);
    game->removePlayer("Lior");
    delete game;
}

TEST_CASE("CurrentPlayer and MoveTurn") {
    Game* game = new Game();

    SUBCASE("No players in the game") {
        CHECK(game->currentPlayer() == nullptr);
    }

    SUBCASE("Regular case with removing") {
        game->addPlayer("Adi");
        game->addPlayer("Noa");
        game->addPlayer("Lior");
        game->addPlayer("Roni");
        Player* p = game->currentPlayer();
        CHECK(p->getUsername() == "Adi");
        game->moveTurn();
        game->moveTurn();
        CHECK(game->currentPlayer()->getUsername() == "Lior");
        game->moveTurn();
        game->moveTurn();
        CHECK(game->currentPlayer()->getUsername() == "Adi");
        game->moveTurn();
        game->moveTurn();
        // Lior's turn
        game->removePlayer("Noa");
        game->moveTurn();
        // Should be Roni's turn
        CHECK(game->currentPlayer()->getUsername() == "Roni");
    }
    delete game;
}

TEST_CASE("Players output") {
    Game* game = new Game();

    game->addPlayer("Adi");
    game->addPlayer("Noa");

    stringstream buffer;
    streambuf* oldCout = cout.rdbuf(buffer.rdbuf());  // replace cout to catch the output

    game->players();

    cout.rdbuf(oldCout);  // regular cout

    string expected = "Adi\nNoa\n";
    CHECK(buffer.str() == expected);
    delete game;
}

TEST_CASE("Winner") {
    Game* game = new Game();

    SUBCASE("The game is not finished") {
        game->addPlayer("Adi");
        game->addPlayer("Noa");
        game->addPlayer("Lior");

        stringstream buffer;
        streambuf* oldCout = cout.rdbuf(buffer.rdbuf());  // replace cout to catch the output

        game->winner();

        cout.rdbuf(oldCout);  // regular cout

        string expected = "Error: The game is not finished yet.\n";
        CHECK(buffer.str() == expected);
    }

    SUBCASE("There is a winner") {
        game->addPlayer("Adi");
        game->addPlayer("Noa");
        game->addPlayer("Lior");

        game->removePlayer("Adi");
        game->removePlayer("Lior");

        stringstream buffer;
        streambuf* oldCout = cout.rdbuf(buffer.rdbuf());  // replace cout to catch the output

        game->winner();

        cout.rdbuf(oldCout);  // regular cout

        string expected = "The game is over! The winner is Noa.\n";
        CHECK(buffer.str() == expected);
    }
    delete game;
}

