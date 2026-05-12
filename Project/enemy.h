#pragma once
#include "Entity.h"
#include <cmath>

class Enemy : public Entity {
protected:
    int maxHealth;
    int currentPoint;
    int reward;
    float animationTimer;
    sf::Vector2f targetPosition;

public:
    float speed;
    int health;
    // FIX: Initialize all members in the constructor to avoid garbage values
    Enemy() : speed(100.0f), health(100),maxHealth(health), currentPoint(0), reward(5), isFinished(false), isDead(false) {}

    bool isFinished;
    bool isDead;

    void takeDamage(int amount) {
        health -= amount;
        if (health <= 0) isDead = true;
    }

    // Base moveOnPath
    virtual void moveOnPath(sf::Vector2f waypoints[], int waypointCount, float dt) {
        if (currentPoint < waypointCount) {
            sf::Vector2f target = waypoints[currentPoint];
            sf::Vector2f dir = target - position;
            float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

            if (dist < 5.0f) {
                currentPoint++;
            }
            else {
                position += (dir / dist) * speed * dt;
                sprite.setPosition(position); // CRITICAL: Updates visual position
            }
        }
        else {
            isFinished = true;
        }
    }

    void drawHealthBar(sf::RenderWindow& window) {
        if (health <= 0) return;

        float barWidth = 40.0f;
        float barHeight = 4.0f;
        // Positioned for 64x64 enemies
        sf::Vector2f barPos = position + sf::Vector2f(7, 0.5);

        sf::RectangleShape backBar(sf::Vector2f(barWidth, barHeight));
        backBar.setFillColor(sf::Color(100, 0, 0)); // Dark red
        backBar.setPosition(barPos);

        float healthRatio = (float)health / (float)maxHealth;
        sf::RectangleShape frontBar(sf::Vector2f(barWidth * healthRatio, barHeight));

        // Dynamic Color: Changes from Green to Yellow to Red as they die
        if (healthRatio > 0.5f) frontBar.setFillColor(sf::Color::Green);
        else if (healthRatio > 0.25f) frontBar.setFillColor(sf::Color::Yellow);
        else frontBar.setFillColor(sf::Color::Red);

        frontBar.setPosition(barPos);

        window.draw(backBar);
        window.draw(frontBar);
    }

    virtual int getReward() const {
        return reward;
    }

    // FIX: All children use 'override' for update, so base MUST be virtual
    virtual void update(float dt) = 0;
};