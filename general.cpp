// noashalom5@gmail.com

#include "general.hpp"
#include "player.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

namespace coup {

    bool General::wantsToUndoCoup() const {
        sf::RenderWindow window(sf::VideoMode(400, 200), "Undo Coup?");
        sf::Font font;
        if (!font.loadFromFile("ubuntu-regular.ttf")) {
            // fallback: always return false
            return false;
        }

        sf::Text question("General " + getUsername() + ",\nUndo the coup?", font, 24);
        question.setPosition(50, 30);
        question.setFillColor(sf::Color::White);

        // Yes button
        sf::RectangleShape yesButton(sf::Vector2f(100, 40));
        yesButton.setPosition(70, 120);
        yesButton.setFillColor(sf::Color::Green);
        sf::Text yesText("Yes", font, 20);
        yesText.setPosition(100, 125);
        yesText.setFillColor(sf::Color::Black);

        // No button
        sf::RectangleShape noButton(sf::Vector2f(100, 40));
        noButton.setPosition(230, 120);
        noButton.setFillColor(sf::Color::Red);
        sf::Text noText("No", font, 20);
        noText.setPosition(265, 125);
        noText.setFillColor(sf::Color::Black);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();

                if (event.type == sf::Event::MouseButtonPressed) {
                    auto mouse = sf::Mouse::getPosition(window);
                    if (yesButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
                        window.close();
                        return true;
                    }
                    if (noButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
                        window.close();
                        return false;
                    }
                }
            }

            window.clear(sf::Color(30, 30, 30));
            window.draw(question);
            window.draw(yesButton);
            window.draw(noButton);
            window.draw(yesText);
            window.draw(noText);
            window.display();
        }

        return false; // fallback
    }

    // bool General::wantsToUndoCoup() const {
    //     string answer;
    //     cout << "General " << getUsername() << ", do you want to undo the coup? (yes/no): ";
    //     cin >> answer;
    //     return answer == "yes";
    // }
}