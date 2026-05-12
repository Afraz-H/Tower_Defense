#pragma once
#include "Entity.h"
#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include <cmath>

class Tower : public Entity {
protected:
    float range;
    float attackSpeed;
    float fireTimer;
    int damage;
    int cost;

    // Range Overlay
    sf::CircleShape rangeCircle;

    // Projectile Management (No vectors, manual array)
    static const int MAX_PROJ = 15;
    sf::Sprite* projectiles[MAX_PROJ];
    sf::Texture projectileTex;
    sf::Vector2f projDirs[MAX_PROJ];
    bool projActive[MAX_PROJ];

public:
    Tower(sf::Vector2f pos) { // Pass pos here
        position = pos;
        sprite.setPosition(position);
        range = 0;
        attackSpeed = 1.0f;
        fireTimer = 10.0f;
        damage = 0;
        cost = 0;

        for (int i = 0; i < MAX_PROJ; i++) {
            projectiles[i] = new sf::Sprite();
            projectiles[i]->setTexture(projectileTex);

            // Set origin to middle so it spins/spawns from its center
            sf::FloatRect bounds = projectiles[i]->getLocalBounds();
            projectiles[i]->setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

            projActive[i] = false;
        }
    }

    int level = 1;
    int basePrice;
    bool isUpgraded = false;

    virtual int getUpgradeCost() { return basePrice * 1.5f; }
    virtual int getSellValue() { return (basePrice + (isUpgraded ? getUpgradeCost() : 0)) * 0.6f; }
    virtual void upgrade() {
        level = 2;
        isUpgraded = true;
        // Logic for range/dmg/speed increases in child classes
    }

    virtual ~Tower() {
        for (int i = 0; i < MAX_PROJ; i++) {
            delete projectiles[i];
        }
    }

    // --- Selection & Visuals ---
    
    bool isSelected = false;
    void setSelected(bool s) { isSelected = s; }
    bool getSelected() const { return isSelected; }
    float getRange() const { return range; }

    void initRangeCircle(sf::Vector2f pos, float r) {
        range = r;
        rangeCircle.setRadius(range);
        rangeCircle.setOrigin(range, range);
        rangeCircle.setPosition(pos + sf::Vector2f(32, 32));

        // ISOMETRIC SQUASH: 1.0 width, 0.7 height
        rangeCircle.setScale(1.0f, 0.7f);

        rangeCircle.setFillColor(sf::Color(255, 255, 255, 25)); // Very transparent
        rangeCircle.setOutlineThickness(1.5f);
        rangeCircle.setOutlineColor(sf::Color(255, 255, 255, 100));
    }

    sf::Sprite& getSprite() {
        return sprite;
    }

    // --- Isometric Math ---
    // This makes the tower's "vision" match the elliptical range circle
    float getDistance(Enemy* e) {
        if (!e) return 9999.0f;
        sf::Vector2f tCenter = position + sf::Vector2f(32, 32);
        sf::Vector2f eCenter = e->position + sf::Vector2f(32, 32);

        float dx = eCenter.x - tCenter.x;
        float dy = eCenter.y - tCenter.y;

        // Weight the Y axis by 0.7 to match the visual ellipse
        return std::sqrt(dx * dx + std::pow(dy * 1.4f, 2));
    }

    // --- Projectile Handling ---
    void spawnProjectile(sf::Vector2f targetPos) {
        for (int i = 0; i < MAX_PROJ; i++) {
            if (!projActive[i]) {
                sf::Vector2f start = position + sf::Vector2f(32, 32);
                projectiles[i]->setPosition(start);

                sf::Vector2f dir = targetPos - start;
                float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                projDirs[i] = (len != 0) ? dir / len : sf::Vector2f(0, 0);

                projActive[i] = true;
                break;
            }
        }
    }

    void updateProjectiles(float dt, Enemy* enemies[], int enemyCount) {
        for (int i = 0; i < MAX_PROJ; i++) {
            if (projActive[i]) {
                projectiles[i]->move(projDirs[i] * 450.0f * dt);

                // Rotation 
                float angle = std::atan2(projDirs[i].y, projDirs[i].x) * 180.0f / 3.14159f;
                projectiles[i]->setRotation(angle);

                // Range check
                float dx = projectiles[i]->getPosition().x - (position.x + 32);
                float dy = projectiles[i]->getPosition().y - (position.y + 32);
                float realDist = std::sqrt(dx * dx + dy * dy);
                if (realDist > range) projActive[i] = false;

                // Simple collision
                for (int j = 0; j < enemyCount; j++) {
                    if (enemies[j] && !enemies[j]->isDead) {
                        if (projectiles[i]->getGlobalBounds().intersects(enemies[j]->sprite.getGlobalBounds())) {
                            enemies[j]->health -= damage;
                            projActive[i] = false;
                            break;
                            if (enemies[j]->health <= 0) {
                                enemies[j]->isDead = true; // Mark for deletion in GameEngine
                            }
                        }
                    }
                }
            }
        }
    }

    // Overloaded helper for projectile range check
    float getDistance(sf::Vector2f targetPos) {
        sf::Vector2f tCenter = position + sf::Vector2f(32, 32);
        float dx = targetPos.x - tCenter.x;
        float dy = targetPos.y - tCenter.y;
        return std::sqrt(dx * dx + std::pow(dy * 1.4f, 2));
    }

    // --- Rendering ---
    // Inside tower.h
    virtual void draw(sf::RenderWindow& window) {
        // FIX: Draw the range circle first so it's under the tower
        if (isSelected) {
            window.draw(rangeCircle);
        }

        window.draw(sprite);

        // Draw active projectiles
        for (int i = 0; i < MAX_PROJ; i++) {
            if (projActive[i]) {
                window.draw(*projectiles[i]);
            }
        }
    }

    // --- Virtual Interface ---
    void update(float dt) override {}
    virtual void update(float dt, Enemy* enemies[], int enemyCount) = 0;
    virtual void attack(Enemy* enemies[], int enemyCount) = 0;

    int getCost() const { return cost; }
    void setDamage(int d) { damage = d; }
    int getDamage() const { return damage; }
    void setAttackSpeed(float s) { attackSpeed = s; }
    float getAttackSpeed() const { return attackSpeed; }
    void upgradeDamage(int amount) { damage += amount; }
    void upgradeSpeed(float factor) { attackSpeed *= factor; }
};