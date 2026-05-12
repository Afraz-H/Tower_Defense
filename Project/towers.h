#pragma once
#include "tower.h"

// --- Cannon ---
class Cannon : public Tower {
private:
    sf::IntRect frameRect;
    int frameWidth = 61;
    int frameHeight = 64;

public:
    Cannon(sf::Vector2f pos) : Tower(pos) {
        position = pos;
        damage = 0;
        attackSpeed = 1.0f;
        range = 150;
        initRangeCircle(pos, 150);

        if (loadTexture("assets/tower0.png")) {
            sprite.setOrigin(0.0f, 0.0f);
            frameRect = sf::IntRect(0, 0, frameWidth, frameHeight);
            sprite.setTextureRect(frameRect);
            sprite.setPosition(pos);
        }

        // Projectile Setup: Cannonball
        projectileTex.loadFromFile("assets/effect1.png");
        sf::IntRect ballFrame(0, 0, 64, 64); // Adjust these 4 numbers to your sheet!

        for (int i = 0; i < MAX_PROJ; i++) {
            projectiles[i]->setTexture(projectileTex);
            projectiles[i]->setTextureRect(ballFrame); // CROPS THE SHEET
            projectiles[i]->setScale(0.2f, 0.2f);
            projectiles[i]->setOrigin(32, 32); // Half of frame width/height
        }
    }

    void update(float dt, Enemy* enemies[], int enemyCount) override {
        fireTimer += dt;
        updateProjectiles(dt, enemies, enemyCount); // Move the balls
        if (fireTimer >= attackSpeed) {
            attack(enemies, enemyCount);
            fireTimer = 0.0f;
        }
    }

    void attack(Enemy* enemies[], int enemyCount) override {
        for (int i = 0; i < enemyCount; i++) {
            if (enemies[i] && !enemies[i]->isDead) {
                if (getDistance(enemies[i]) <= range) {
                    // Spawn physical projectile
                    spawnProjectile(enemies[i]->position + sf::Vector2f(32, 32));

                    // Rotate Cannon Sprite (Animation logic)
                    sf::Vector2f towerCenter = position + sf::Vector2f(32, 32);
                    sf::Vector2f eCenter = enemies[i]->position + sf::Vector2f(32, 32);
                    float angle = std::atan2(eCenter.y - towerCenter.y, eCenter.x - towerCenter.x) * 180.0f / 3.14159f;
                    float clockwiseAngle = 180.0f + angle;
                    int frameIndex = static_cast<int>((clockwiseAngle + 22.5f) / 45.0f) % 8;
                    frameRect.left = frameIndex * frameWidth;
                    sprite.setTextureRect(frameRect);
                    break;
                }
            }
        }
    }
};

// --- MachineGun ---
class MachineGun : public Tower {
private:
    sf::IntRect frameRect;
    int frameWidth = 57;
    int frameHeight = 64;

public:
    MachineGun(sf::Vector2f pos) : Tower(pos) {
        position = pos;
        damage = 0;
        attackSpeed = 0.15f;
        range = 200.0f;
        initRangeCircle(pos, range);

        if (loadTexture("assets/tower1.png")) {
            frameRect = sf::IntRect(0, 0, frameWidth, frameHeight);
            sprite.setTextureRect(frameRect);
            sprite.setPosition(pos);
        }

        // Projectile Setup: Small Arrow
        projectileTex.loadFromFile("assets/effect2.png");
        sf::IntRect arrowFrame(0, 0, 64, 64); // Adjust these 4 numbers to your sheet!

        for (int i = 0; i < MAX_PROJ; i++) {
            projectiles[i]->setTexture(projectileTex);
            projectiles[i]->setTextureRect(arrowFrame); // CROPS THE SHEET
            projectiles[i]->setScale(0.6f, 0.6f);
            projectiles[i]->setOrigin(32, 32); // Half of frame width/height
        }
    }

    void update(float dt, Enemy* enemies[], int enemyCount) override {
        fireTimer += dt;
        updateProjectiles(dt, enemies, enemyCount);
        if (fireTimer >= attackSpeed) {
            attack(enemies, enemyCount);
            fireTimer = 0.0f;
        }
    }

    void attack(Enemy* enemies[], int enemyCount) override {
        for (int i = 0; i < enemyCount; i++) {
            if (enemies[i] && !enemies[i]->isDead) {
                if (getDistance(enemies[i]) <= range) {
                    spawnProjectile(enemies[i]->position + sf::Vector2f(32, 32));

                    // Rotation logic
                    sf::Vector2f eCenter = enemies[i]->position + sf::Vector2f(32, 32);
                    sf::Vector2f tCenter = position + sf::Vector2f(32, 32);
                    float angle = std::atan2(eCenter.y - tCenter.y, eCenter.x - tCenter.x) * 180.0f / 3.14159f;
                    int frameIndex = static_cast<int>((180.0f + angle + 22.5f) / 45.0f) % 8;
                    frameRect.left = frameIndex * frameWidth;
                    sprite.setTextureRect(frameRect);
                    break;
                }
            }
        }
    }
};

// --- Sniper ---
class Sniper : public Tower {
private:
    sf::IntRect frameRect;
    int frameWidth = 48;
    int frameHeight = 64;

public:
    Sniper(sf::Vector2f pos) : Tower(pos) {
        position = pos;
        damage = 0;
        attackSpeed = 3.0f;
        range = 550.0f;
        initRangeCircle(pos, range);

        if (loadTexture("assets/tower2.png")) {
            frameRect = sf::IntRect(0, 0, frameWidth, frameHeight);
            sprite.setTextureRect(frameRect);
            sprite.setPosition(pos);
        }

        // Projectile Setup: Big Arrow
        projectileTex.loadFromFile("assets/effect2.png");
        sf::IntRect arrowFrame(0, 0, 64, 64); // Adjust these 4 numbers to your sheet!

        for (int i = 0; i < MAX_PROJ; i++) {
            projectiles[i]->setTexture(projectileTex);
            projectiles[i]->setTextureRect(arrowFrame); // CROPS THE SHEET
            projectiles[i]->setScale(1.0f, 1.0f);
            projectiles[i]->setOrigin(32, 32); // Half of frame width/height
        }
    }

    void update(float dt, Enemy* enemies[], int enemyCount) override {
        fireTimer += dt;
        updateProjectiles(dt, enemies, enemyCount);
        if (fireTimer >= attackSpeed) {
            attack(enemies, enemyCount);
            fireTimer = 0.0f;
        }
    }

    void attack(Enemy* enemies[], int enemyCount) override {
        for (int i = 0; i < enemyCount; i++) {
            if (enemies[i] && !enemies[i]->isDead) {
                // Use the base class getDistance which handles Isometric scaling
                if (getDistance(enemies[i]) <= range) {

                    // 1. SHOOT
                    spawnProjectile(enemies[i]->position + sf::Vector2f(32, 32));

                    // 2. ROTATE THE TOWER (The missing part)
                    sf::Vector2f towerCenter = position + sf::Vector2f(32, 32);
                    sf::Vector2f eCenter = enemies[i]->position + sf::Vector2f(32, 32);

                    float dx = eCenter.x - towerCenter.x;
                    float dy = eCenter.y - towerCenter.y;
                    float angle = std::atan2(dy, dx) * 180.0f / 3.14159f;

                    // Standardize angle to 0-360
                    float clockwiseAngle = 180.0f + angle;
                    while (clockwiseAngle < 0) clockwiseAngle += 360;
                    while (clockwiseAngle >= 360) clockwiseAngle -= 360;

                    // Pick the correct frame from your 8-direction sprite sheet
                    int frameIndex = static_cast<int>((clockwiseAngle + 22.5f) / 45.0f) % 8;
                    frameRect.left = frameIndex * frameWidth;
                    sprite.setTextureRect(frameRect);

                    break; // Stop looking after firing one shot
                }
            }
        }
    }
};

// --- FreezeTower ---
class FreezeTower : public Tower {
public:
    FreezeTower(sf::Vector2f pos) : Tower(pos) {
        position = pos;
        damage = 0;           // Freeze towers usually don't deal damage
        attackSpeed = 0.5f;   // How often it reapplies the slow
        range = 180.0f;
        initRangeCircle(pos, range);

        if (loadTexture("assets/tower3.png")) {
            sprite.setPosition(pos);
            sprite.setScale(0.23f, 0.23f);
        }
    }

    void update(float dt, Enemy* enemies[], int enemyCount) override {
        fireTimer += dt;
        // Pulse logic: Every 0.5 seconds, catch everyone in range
        if (fireTimer >= attackSpeed) {
            attack(enemies, enemyCount);
            fireTimer = 0.0f;
        }
    }

    void attack(Enemy* enemies[], int enemyCount) override {
        for (int i = 0; i < enemyCount; i++) {
            if (enemies[i] && !enemies[i]->isDead) {
                if (getDistance(enemies[i]) <= range) {
                    // Apply 20% slow
                    enemies[i]->speed *= 0.8f;

                    // Cap the minimum speed so they don't stop entirely
                    float minSpeed = 30.0f;
                    if (enemies[i]->speed < minSpeed) {
                        enemies[i]->speed = minSpeed;
                    }
                }
            }
        }
    }
};

// --- Flamethrower (AOE Logic) ---
class Flamethrower : public Tower {
private:
    sf::Sprite flameSprite;
    sf::Texture flameTexture;
    bool showFlames = false;
    float flameDisplayTimer = 0.0f;

public:
    Flamethrower(sf::Vector2f pos) : Tower(pos) {
        position = pos;
        range = 130.0f;
        attackSpeed = 0.1f;
        damage = 2; // Low damage but hits everyone
        initRangeCircle(pos, range);

        loadTexture("assets/tower4.png");
        sprite.setPosition(position);
        sprite.setScale(0.22, 0.22f);

        if (flameTexture.loadFromFile("assets/effect0.png")) {
            flameSprite.setTexture(flameTexture);
            sf::FloatRect b = flameSprite.getLocalBounds();
            flameSprite.setOrigin(0, b.height / 1.25f); // Origin at back of flame
        }
    }

    void update(float dt, Enemy* enemies[], int enemyCount) override {
        fireTimer += dt;
        if (showFlames) {
            flameDisplayTimer -= dt;
            if (flameDisplayTimer <= 0) showFlames = false;
        }
        if (fireTimer >= attackSpeed) {
            attack(enemies, enemyCount);
            fireTimer = 0.0f;
        }
    }

    void attack(Enemy* enemies[], int enemyCount) override {
        sf::Vector2f towerCenter = position + sf::Vector2f(32, 32);
        bool hitAnything = false;

        // AOE LOOP: Check ALL enemies in range
        for (int i = 0; i < enemyCount; i++) {
            if (enemies[i] && !enemies[i]->isDead) {
                if (getDistance(enemies[i]) <= range) {
                    enemies[i]->health -= damage;

                    // Look at the first guy we hit to aim the flame
                    if (!hitAnything) {
                        sf::Vector2f eCenter = enemies[i]->position + sf::Vector2f(32, 32);
                        float angle = std::atan2(eCenter.y - towerCenter.y, eCenter.x - towerCenter.x);
                        flameSprite.setRotation(angle * 180.0f / 3.14159f);
                    }

                    hitAnything = true;
                    if (enemies[i]->health <= 0) enemies[i]->isDead = true;
                }
            }
        }

        if (hitAnything) {
            showFlames = true;
            flameDisplayTimer = 0.15f;
        }
    }

    void draw(sf::RenderWindow& window) override {
        if (isSelected) window.draw(rangeCircle);
        window.draw(sprite);
        if (showFlames) {
            flameSprite.setPosition(position + sf::Vector2f(32, 32));
            window.draw(flameSprite);
        }
    }
};

// --- Elemental Spawner ---
class ElementalSpawn {
public:
    sf::CircleShape body;
    sf::Vector2f pos;
    bool active = false;
    float speed = 250.0f;

    ElementalSpawn() {
        body.setRadius(8.0f);
        body.setFillColor(sf::Color::Cyan);
        body.setOrigin(8, 8);
    }

    void moveTowards(sf::Vector2f target, float dt) {
        sf::Vector2f dir = target - pos;
        float len = sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len > 0) dir /= len;
        pos += dir * speed * dt;
        body.setPosition(pos);
    }
};

class SpawnerTower : public Tower {
private:
    ElementalSpawn spawns[5]; // Manual array of 5 summons
    float spawnTimer = 0.0f;

public:
    SpawnerTower(sf::Vector2f pos) : Tower(pos) {
        position = pos;
        damage = 50;
        attackSpeed = 4.0f; // Time to spawn a new elemental
        range = 300.0f;
        initRangeCircle(pos, range);

        if (loadTexture("assets/tower4.png")) {
            sprite.setPosition(pos);
        }
    }

    void update(float dt, Enemy* enemies[], int enemyCount) override {
        spawnTimer += dt;

        // 1. Try to spawn a new elemental if one is available
        if (spawnTimer >= attackSpeed) {
            for (int i = 0; i < 5; i++) {
                if (!spawns[i].active) {
                    spawns[i].pos = position + sf::Vector2f(32, 32);
                    spawns[i].active = true;
                    spawnTimer = 0.0f;
                    break;
                }
            }
        }

        // 2. Update existing elementals
        for (int i = 0; i < 5; i++) {
            if (spawns[i].active) {
                Enemy* target = nullptr;
                float minDist = 9999.0f;

                // Find the closest enemy using Isometric Math
                for (int j = 0; j < enemyCount; j++) {
                    if (enemies[j] && !enemies[j]->isDead) {
                        float d = getDistance(enemies[j]);
                        if (d < minDist && d <= range) {
                            minDist = d;
                            target = enemies[j];
                        }
                    }
                }

                if (target) {
                    // Move towards target
                    spawns[i].moveTowards(target->position + sf::Vector2f(32, 32), dt);

                    // Check collision (Standard distance is fine for touch)
                    float hitDist = sqrt(pow(spawns[i].pos.x - (target->position.x + 32), 2) +
                        pow(spawns[i].pos.y - (target->position.y + 32), 2));

                    if (hitDist < 15.0f) {
                        target->health -= damage;
                        if (target->health <= 0) target->isDead = true;
                        spawns[i].active = false; // Elemental "explodes" on impact
                    }
                }
                else {
                    // If no target, the elemental stays near the tower
                    spawns[i].moveTowards(position + sf::Vector2f(32, 32), dt * 0.5f);
                }
            }
        }
    }

    void draw(sf::RenderWindow& window) override {
        if (isSelected) window.draw(rangeCircle);
        window.draw(sprite);
        for (int i = 0; i < 5; i++) {
            if (spawns[i].active) window.draw(spawns[i].body);
        }
    }

    // Required override but not used since update handles it
    void attack(Enemy* enemies[], int enemyCount) override {}
};