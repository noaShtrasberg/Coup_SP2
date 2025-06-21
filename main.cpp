// noashalom5@gmail.com

#include "game.hpp"
#include "player.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;
using namespace coup;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

sf::Font loadFont() {
    sf::Font font;
    if (!font.loadFromFile("Ubuntu-Regular.ttf")) {
        cerr << "Error: Could not load font Ubuntu-Regular.ttf" << endl;
        exit(1);
    }
    return font;
}

void drawTextBox(sf::RenderWindow& window, const vector<string>& lines, sf::Font& font) {
    window.clear(sf::Color::Black);
    int y = 10;
    for (const string& line : lines) {
        sf::Text text(line, font, 20);
        text.setPosition(10, y);
        text.setFillColor(sf::Color::White);
        window.draw(text);
        y += 25;
    }
}

void showErrorPopup(sf::Font& font, const string& message) {
    sf::RectangleShape popup(sf::Vector2f(500, 150));
    popup.setFillColor(sf::Color(200, 50, 50));
    popup.setPosition((WINDOW_WIDTH - 500) / 2, (WINDOW_HEIGHT - 150) / 2);

    sf::Text text(message, font, 22);
    text.setFillColor(sf::Color::White);
    text.setPosition(popup.getPosition().x + 20, popup.getPosition().y + 50);

    sf::RenderWindow popupWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Error", sf::Style::None);
    while (popupWindow.isOpen()) {
        sf::Event event;
        while (popupWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed || event.type == sf::Event::MouseButtonPressed)
                popupWindow.close();
        }
        popupWindow.clear(sf::Color::Transparent);
        popupWindow.draw(popup);
        popupWindow.draw(text);
        popupWindow.display();
    }
}

sf::RectangleShape createButton(const string& label, sf::Font& font, float x, float y, float width, float height, sf::Text& text) {
    sf::RectangleShape button(sf::Vector2f(width, height));
    button.setPosition(x, y);
    button.setFillColor(sf::Color(100, 100, 250));

    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setPosition(x + 10, y + 10);

    return button;
}

vector<Player*> promptPlayerSelection(sf::RenderWindow& window, sf::Font& font, const vector<Player*>& players, const string& prompt) {
    vector<Player*> targets;
    vector<sf::RectangleShape> buttons;
    vector<sf::Text> texts;

    for (size_t i = 0; i < players.size(); ++i) {
        sf::Text text;
        buttons.push_back(createButton(players[i]->getUsername(), font, 300, 50 + i * 60, 200, 40, text));
        texts.push_back(text);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                for (size_t i = 0; i < buttons.size(); ++i) {
                    if (buttons[i].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        targets.push_back(players[i]);
                        return targets;
                    }
                }
            }
        }

        window.clear();
        sf::Text promptText(prompt, font, 24);
        promptText.setPosition(10, 10);
        promptText.setFillColor(sf::Color::White);
        window.draw(promptText);

        for (auto& btn : buttons) window.draw(btn);
        for (auto& txt : texts) window.draw(txt);

        window.display();
    }
    return targets;
}

int promptNumPlayers(sf::RenderWindow& window, sf::Font& font) {
    vector<sf::RectangleShape> buttons;
    vector<sf::Text> texts;
    int selected = 0;

    for (int i = 2; i <= 6; ++i) {
        sf::Text text;
        buttons.push_back(createButton(to_string(i), font, 100 + (i - 2) * 110, 250, 100, 50, text));
        texts.push_back(text);
    }

    sf::Text title("Select number of players:", font, 28);
    title.setPosition(100, 180);
    title.setFillColor(sf::Color::White);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                for (size_t i = 0; i < buttons.size(); ++i) {
                    if (buttons[i].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        selected = i + 2;
                        return selected;
                    }
                }
            }
        }

        window.clear();
        window.draw(title);
        for (auto& btn : buttons) window.draw(btn);
        for (auto& txt : texts) window.draw(txt);
        window.display();
    }
    return selected;
}

string promptNameInput(sf::RenderWindow& window, sf::Font& font, int playerNumber) {
    string name = "";
    sf::Text inputText("", font, 24);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(100, 200);

    sf::Text promptText("Enter name for player " + to_string(playerNumber) + ":", font, 24);
    promptText.setFillColor(sf::Color::White);
    promptText.setPosition(100, 150);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 13 && !name.empty()) return name; // Enter
                else if (event.text.unicode == 8 && !name.empty()) name.pop_back(); // Backspace
                else if (event.text.unicode < 128 && isprint(event.text.unicode)) name += static_cast<char>(event.text.unicode);
                inputText.setString(name);
            }
        }

        window.clear();
        window.draw(promptText);
        window.draw(inputText);
        window.display();
    }
    return name;
}

int main() {
    Game game;
    sf::Font font = loadFont();
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Coup Game GUI");
    vector<string> log;

    bool gameStarted = false;
    sf::Text startText;
    sf::RectangleShape startButton = createButton("Start Game", font, 300, 400, 200, 50, startText);

    while (window.isOpen() && !gameStarted) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (startButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    gameStarted = true;
                }
            }
        }
        window.clear();
        window.draw(startButton);
        window.draw(startText);
        window.display();
    }

    int numPlayers = promptNumPlayers(window, font);

    for (int i = 0; i < numPlayers; i++) {
        string name = promptNameInput(window, font, i + 1);
        game.addPlayer(name);
        log.push_back("Player " + name + " joined the game");
    }

    log.push_back("Final player list order:");
    for (const auto& p : game.getPlayersList()) {
        log.push_back(p->getUsername());
    }


    log.push_back("Game started!");

    while (window.isOpen() && game.getPlayersList().size() > 1) {
        Player* current = game.currentPlayer();
        log.push_back("It's " + current->getUsername() + "'s turn. Coins: " + to_string(current->getCoins()));
        log.push_back("== Turn Index: " + to_string(game.getPlayerTurnIndex()) + " ==");

        vector<string> actions = {"Gather", "Tax", "Bribe", "Arrest", "Sanction", "Coup", "Skip"};
        if (current->getRole() == "Baron" || current->getRole() == "Spy")
            actions.push_back("Special Turn");

        vector<sf::RectangleShape> buttons;
        vector<sf::Text> texts;

        for (size_t i = 0; i < actions.size(); ++i) {
            sf::Text text;
            buttons.push_back(createButton(actions[i], font, 50, 50 + i * 60, 200, 40, text));
            texts.push_back(text);
        }

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) window.close();
                else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    for (size_t i = 0; i < buttons.size(); ++i) {
                        if (buttons[i].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                            string action = actions[i];
                            try {
                                if (action == "Gather") current->gather();
                                else if (action == "Tax") current->tax();
                                else if (action == "Bribe") current->bribe();
                                else if (action == "Arrest") {
                                    vector<Player*> target = promptPlayerSelection(window, font, game.getPlayersList(), "Select player to arrest");
                                    if (!target.empty()) current->arrest(target[0]);
                                } else if (action == "Sanction") {
                                    vector<Player*> target = promptPlayerSelection(window, font, game.getPlayersList(), "Select player to sanction");
                                    if (!target.empty()) current->sanction(target[0]);
                                } else if (action == "Coup") {
                                    vector<Player*> target = promptPlayerSelection(window, font, game.getPlayersList(), "Select player to coup");
                                    if (!target.empty()) current->coup(target[0]);
                                } else if (action == "Skip") current->skipTurn();
                                else if (action == "Special Turn") current->specialTurn();
                                goto next_turn;
                            } catch (const exception& e) {
                                showErrorPopup(font, e.what());
                            }
                        }
                    }
                }
            }

            window.clear();
            drawTextBox(window, log, font);
            for (auto& btn : buttons) window.draw(btn);
            for (auto& txt : texts) window.draw(txt);
            window.display();
        }

    next_turn:
        if (log.size() > 20) log.erase(log.begin());
    }

    log.push_back("The winner is: " + game.getPlayersList().front()->getUsername());

    sf::Text returnText;
    sf::RectangleShape returnButton = createButton("Back to Main Menu", font, 250, 500, 300, 50, returnText);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (returnButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    main(); // restart game from main menu
                    return 0;
                }
            }
        }
        window.clear();
        drawTextBox(window, log, font);
        window.draw(returnButton);
        window.draw(returnText);
        window.display();
    }

    return 0;
}
