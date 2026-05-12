#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameEngine.h"
#include "Menu.h"

using namespace std;

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Tower Defense");
    window.setFramerateLimit(60);

    MainMenu menu(1280, 720);
    GameState state = GameState::Menu;

    while (window.isOpen()) {
        sf::Event event;
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Pass the entire event to the menu for Keyboard + Mouse support
            menu.handleInput(event, state);

            // Manual Mouse Click Fallback (if you didn't add it to handleInput)
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                // If you want mouse clicks to still work, ensure handleInput 
                // in Menu.h checks mouse bounds too!
            }
        }

        if (state == GameState::Menu) {
            menu.update(mousePos);
            window.clear(sf::Color(30, 30, 30));
            menu.draw(window);
            window.display();
        }
        else if (state == GameState::Quit) {
            window.close();
        }
        else {
            // Start the Game Engine
            GameEngine engine(window, state);
            engine.run();

            // Return to menu after game ends
            state = GameState::Menu;
        }
    }

    return 0;
}