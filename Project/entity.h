#pragma once
#include <SFML/Graphics.hpp>

class Entity {
public:
    sf::Vector2f position;
    sf::Sprite sprite;
    sf::Texture texture;

    // Pure virtual: All entities must have a basic update
    virtual void update(float dt) = 0;

    sf::Vector2f getPosition() { return position; }

    bool loadTexture(const std::string& filename) {
        if (!texture.loadFromFile(filename)) {
            return false; // Error loading file
        }
        sprite.setTexture(texture);
        return true;
    }

    virtual void draw(sf::RenderWindow& window) {
        sprite.setPosition(position);
        window.draw(sprite);
    }
};