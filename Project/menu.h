#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class GameState { Menu, NormalMode, EndlessMode, Settings, Instructions, Quit };

class MainMenu {
private:
    // Internal UI States (Inspired by your tank game)
    enum class SubState { MAIN, SETTINGS, INSTRUCTIONS };
    SubState subState = SubState::MAIN;

    sf::Font font;
    static const int MAX_ITEMS = 5;
    int selectedIndex = 0; // For Keyboard Navigation

    sf::Text menuItems[MAX_ITEMS];
    sf::Text instructionLines[10]; // For the instruction screen

    // Graphical Elements
    sf::Texture bgTexture;
    sf::Sprite bgSprite;
    sf::RectangleShape panel; // Semi-transparent overlay for sub-menus

    const char* labels[MAX_ITEMS] = {
        "Normal Mode", "Endless Mode", "Settings", "Instructions", "Exit"
    };

    // Settings Variables
    int difficulty = 1; // 0: Easy, 1: Normal, 2: Hard
    const std::string diffLabels[3] = { "Easy", "Normal", "Hard" };

public:
    MainMenu(float width, float height) {
        font.loadFromFile("C:/Windows/Fonts/arial.ttf");

        // Load Background (Placeholder - adjust to your path)
        if (bgTexture.loadFromFile("assets/bg_menu0.png")) {
            bgSprite.setTexture(bgTexture);
            bgSprite.setScale(width / bgTexture.getSize().x, height / bgTexture.getSize().y);
        }

        // Setup Main Buttons
        for (int i = 0; i < MAX_ITEMS; i++) {
            menuItems[i].setFont(font);
            menuItems[i].setString(labels[i]);
            menuItems[i].setCharacterSize(45);
            setupTextOrigin(menuItems[i]);
            menuItems[i].setPosition(width / 2.0f, height / (MAX_ITEMS + 1) * (i + 1));
        }

        // Setup Instruction Text (Static content)
        std::string instructions[] = { "CONTROLS:", "Left Click: Select/Place", "Radial Menu: Choose Tower", "S Key: Start Wave", "ESC: Pause" };
        for (int i = 0; i < 5; i++) {
            instructionLines[i].setFont(font);
            instructionLines[i].setString(instructions[i]);
            instructionLines[i].setCharacterSize(24);
            instructionLines[i].setPosition(width / 4.0f, 200 + (i * 40));
        }

        // Setup Panel (Dark overlay for sub-menus)
        panel.setSize(sf::Vector2f(width * 0.8f, height * 0.8f));
        panel.setOrigin(panel.getSize().x / 2.0f, panel.getSize().y / 2.0f);
        panel.setPosition(width / 2.0f, height / 2.0f);
        panel.setFillColor(sf::Color(0, 0, 0, 200));
    }

    void setupTextOrigin(sf::Text& t) {
        sf::FloatRect b = t.getLocalBounds();
        t.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
    }

    void handleInput(sf::Event event, GameState& currentState) {
        if (event.type == sf::Event::KeyPressed) {
            // BACK logic
            if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::BackSpace) {
                subState = SubState::MAIN;
            }

            if (subState == SubState::MAIN) {
                if (event.key.code == sf::Keyboard::Up) selectedIndex = (selectedIndex - 1 + MAX_ITEMS) % MAX_ITEMS;
                if (event.key.code == sf::Keyboard::Down) selectedIndex = (selectedIndex + 1) % MAX_ITEMS;

                if (event.key.code == sf::Keyboard::Enter) {
                    if (selectedIndex == 0) currentState = GameState::NormalMode;
                    if (selectedIndex == 1) currentState = GameState::EndlessMode;
                    if (selectedIndex == 2) subState = SubState::SETTINGS;
                    if (selectedIndex == 3) subState = SubState::INSTRUCTIONS;
                    if (selectedIndex == 4) currentState = GameState::Quit;
                }
            }
            else if (subState == SubState::SETTINGS) {
                if (event.key.code == sf::Keyboard::Left) difficulty = (difficulty - 1 + 3) % 3;
                if (event.key.code == sf::Keyboard::Right) difficulty = (difficulty + 1) % 3;
                if (event.key.code == sf::Keyboard::Enter) subState = SubState::MAIN;
            }
        }
    }

    void update(sf::Vector2f mousePos) {
        if (subState == SubState::MAIN) {
            for (int i = 0; i < MAX_ITEMS; i++) {
                if (menuItems[i].getGlobalBounds().contains(mousePos) || i == selectedIndex) {
                    menuItems[i].setFillColor(sf::Color::Yellow);
                    menuItems[i].setScale(1.1f, 1.1f);
                }
                else {
                    menuItems[i].setFillColor(sf::Color::White);
                    menuItems[i].setScale(1.0f, 1.0f);
                }
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(bgSprite);

        if (subState == SubState::MAIN) {
            for (int i = 0; i < MAX_ITEMS; i++) window.draw(menuItems[i]);
        }
        else {
            window.draw(panel);
            if (subState == SubState::INSTRUCTIONS) {
                for (int i = 0; i < 5; i++) window.draw(instructionLines[i]);
            }
            else if (subState == SubState::SETTINGS) {
                sf::Text setTxt;
                setTxt.setFont(font);
                setTxt.setString("DIFFICULTY: < " + diffLabels[difficulty] + " >");
                setTxt.setPosition(window.getSize().x / 2.0f - 150, window.getSize().y / 2.0f);
                window.draw(setTxt);
            }
        }
    }

    int getDifficulty() const { return difficulty; }
};