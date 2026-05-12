#pragma once
#include "enemy.h"
#include <cmath>

// 1. Basic Enemy 1 (Normal)
class BasicEnemy1 : public Enemy {
private:
    sf::IntRect frameRect;
    int currentFrame = 0;
    int totalFrames = 7;
    float animationTimer = 0;
    float frameDuration = 0.15f;
public:
    BasicEnemy1(sf::Vector2f start) {
        position = start; speed = 70.0f; health = 250; maxHealth = health;
        isDead = false; isFinished = false;
        if (loadTexture("assets/enemy0.png")) {
            frameRect = sf::IntRect(0, 0, 64, 64);
            sprite.setTextureRect(frameRect);
        }
    }
    void update(float dt) override {
        animationTimer += dt;
        if (animationTimer >= frameDuration) {
            animationTimer = 0;
            currentFrame = (currentFrame + 1) % totalFrames;
            frameRect.left = currentFrame * 64;
            sprite.setTextureRect(frameRect);
        }
    }
};

// 2. Basic Enemy 2 (Harder)
class BasicEnemy2 : public Enemy {
private:
    sf::IntRect frameRect;
    int currentFrame = 0;
    int totalFrames = 7;
    float animationTimer = 0;
    float frameDuration = 0.15f;
public:
    BasicEnemy2(sf::Vector2f start) {
        position = start; speed = 65.0f; health = 350; maxHealth = health;
        isDead = false; isFinished = false;
        if (loadTexture("assets/enemy1.png")) {
            frameRect = sf::IntRect(0, 0, 64, 64);
            sprite.setTextureRect(frameRect);
        }
    }
    void update(float dt) override {
        animationTimer += dt;
        if (animationTimer >= frameDuration) {
            animationTimer = 0;
            currentFrame = (currentFrame + 1) % totalFrames;
            frameRect.left = currentFrame * 64;
            sprite.setTextureRect(frameRect);
        }
    }
};

// 3. Fast Enemy (Squishy but quick)
class FastEnemy : public Enemy {
private:
    sf::IntRect frameRect;
    int currentFrame = 0;
    int totalFrames = 6;
    float animationTimer = 0;
    float frameDuration = 0.07f; // Half the duration for faster feet
public:
    FastEnemy(sf::Vector2f start) {
        position = start; speed = 140.0f; health = 150; maxHealth = health;
        isDead = false; isFinished = false;
        if (loadTexture("assets/enemy2.png")) {
            frameRect = sf::IntRect(0, 0, 81, 64);
            sprite.setTextureRect(frameRect);
            sprite.setColor(sf::Color(200, 255, 200)); // Green tint
        }
    }
    void update(float dt) override {
        animationTimer += dt;
        if (animationTimer >= frameDuration) {
            animationTimer = 0;
            currentFrame = (currentFrame + 1) % totalFrames;
            frameRect.left = currentFrame * 81;
            sprite.setTextureRect(frameRect);
        }
    }
};

// 4. Tank Enemy (Sponge)
class TankEnemy : public Enemy {
private:
    sf::IntRect frameRect;
    int currentFrame = 0;
    int totalFrames = 10;
    float animationTimer = 0;
    float frameDuration = 0.2f;
public:
    TankEnemy(sf::Vector2f start) {
        position = start; speed = 40.0f; health = 1500; maxHealth = health;
        isDead = false; isFinished = false;
        if (loadTexture("assets/enemy3.png")) {
            frameRect = sf::IntRect(0, 3, 90, 64);
            sprite.setTextureRect(frameRect);        }
    }
    void update(float dt) override {
        animationTimer += dt;
        if (animationTimer >= frameDuration) {
            animationTimer = 0;
            currentFrame = (currentFrame + 1) % totalFrames;
            frameRect.left = currentFrame * 90;
            sprite.setTextureRect(frameRect);
        }
    }
};

// 5. Flying Enemy (Shortcuts the path)
class FlyingEnemy : public Enemy {
public:
    FlyingEnemy(sf::Vector2f start) {
        position = start; speed = 90.0f; health = 250; maxHealth = health;
        isDead = false; isFinished = false;
        if (loadTexture("assets/enemy4.png")) {
            sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
            sprite.setColor(sf::Color::Yellow);
        }
    }

    void update(float dt) override {
        // Simple hover animation
        float bob = std::sin(animationTimer * 5.0f) * 0.5f;
        sprite.move(0, bob);
    }

    // Flying behavior: Ignore path, go straight for the exit
    void moveOnPath(sf::Vector2f path[], int totalPoints, float dt) override {
        sf::Vector2f finalExit = path[totalPoints - 1];
        sf::Vector2f dir = finalExit - position;
        float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        if (dist < 5.0f) {
            isFinished = true;
        }
        else {
            position += (dir / dist) * speed * dt;
            sprite.setPosition(position);
        }
    }
};

// 6. Mid-Boss Enemy (Balanced)
class MidBoss : public Enemy {
private:
    sf::IntRect frameRect;
    int currentFrame = 0;
    int totalFrames = 13;
    float animationTimer = 0;
    float frameDuration = 0.15f;
public:
    MidBoss(sf::Vector2f start) {
        position = start; speed = 60.0f; health = 800; maxHealth = health; reward = 20;
        isDead = false; isFinished = false;
        if (loadTexture("assets/enemy5.png")) {
            frameRect = sf::IntRect(0, 0, 64, 64);
            sprite.setTextureRect(frameRect);
            sprite.setColor(sf::Color(180, 180, 255)); // Blueish armor tint
        }
    }
    void update(float dt) override {
        animationTimer += dt;
        if (animationTimer >= frameDuration) {
            animationTimer = 0;
            currentFrame = (currentFrame + 1) % totalFrames;
            frameRect.left = currentFrame * 64;
            sprite.setTextureRect(frameRect);
        }
    }
};

// 7. Boss Enemy (The End Goal)
class BossEnemy : public Enemy {
private:
    sf::IntRect frameRect;
    int currentFrame = 0;
    int totalFrames = 7;
    float animationTimer = 0;
    float frameDuration = 0.25f;
public:
    BossEnemy(sf::Vector2f start) {
        position = start; speed = 30.0f; health = 6000; maxHealth = health;
        isDead = false; isFinished = false;
        if (loadTexture("assets/boss.png")) {
            frameRect = sf::IntRect(0, 0, 64, 64);
            sprite.setTextureRect(frameRect);
            sprite.setScale(3.0f, 3.0f); // Massive size
            sprite.setColor(sf::Color(255, 100, 100)); // Dark Red
        }
    }
    void update(float dt) override {
        animationTimer += dt;
        if (animationTimer >= frameDuration) {
            animationTimer = 0;
            currentFrame = (currentFrame + 1) % totalFrames;
            frameRect.left = currentFrame * 64;
            sprite.setTextureRect(frameRect);
        }
    }
};