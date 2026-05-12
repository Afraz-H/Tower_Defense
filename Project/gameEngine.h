#pragma once
#include "map.h"
#include "menu.h"
#include "enemy.h"
#include "enemies.h"
#include "tower.h"
#include "towers.h"
#include "assets.h" 
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <cmath>

class GameEngine {
private:
    enum class PlayState { Playing, Won, Lost };
    PlayState playState = PlayState::Playing;

    sf::RenderWindow& window;
    sf::Font font;
    GameState mode;
    Map myMap;

    static const int MAX_ENEMIES = 150;
    static const int MAX_TOWERS = 100;
    Enemy* enemies[MAX_ENEMIES];
    Tower* towers[MAX_TOWERS];

    int playerLives = 20;
    int playerMoney = 300;
    int wave = 1;
    bool isPaused = false;
    float gameSpeed = 1.0f;
    int towerCount = 0;
    int enemyCount = 0;
    int currentLevel = 1;
    int currentTheme = 0;
    int totalLevels = 20;
    int totalWavesInLevel = 6;
    bool isEndlessMode = false;

    // --- NEW UI & UPGRADE VARIABLES ---
    Tower* selectedTower = nullptr;
    sf::RectangleShape upgradeDmgBtn;
    sf::RectangleShape upgradeSpdBtn;
    sf::RectangleShape nukeBtn;
    sf::RectangleShape buffBtn;
    sf::RectangleShape pauseBtn;
    sf::RectangleShape speed2xBtn;

    // Radial Menu Variables
    int towerPrices[5] = { 100, 150, 250, 300, 500 };
    float towerRanges[5] = { 150.0f, 200.0f, 500.0f, 180.0f, 120.0f };
    bool isRadialMenuOpen = false;
    bool isActionMenuOpen = false;
    sf::Vector2i activeGridPos;
    sf::Vector2f menuCenter;
    float radialRadius = 85.0f;
    float menuScale = 0.0f;

    // Wave/Spawn Variables
    float spawnTimer = 0.0f;
    float timeBetweenSpawns = 1.0f;
    bool isWaveActive = false;
    int currentWave = 0;
    int enemiesSpawnedInWave = 0;
    int enemiesToSpawnThisWave = 0;

    int selectedTowerType = 0;

    void prepareWave() {
        enemiesSpawnedInWave = 0;
        int baseSize = 3 + (currentTheme * 5) + (currentLevel % 5);
        enemiesToSpawnThisWave = baseSize + (currentWave * 4);
        timeBetweenSpawns = 1.2f - (currentTheme * 0.15f) - (currentWave * 0.05f);
        if (timeBetweenSpawns < 0.4f) timeBetweenSpawns = 0.4f;
    }

    void spawnEnemyLogic() {
        if (enemyCount >= MAX_ENEMIES) return;
        sf::Vector2f spawnPos = myMap.waypoints[0];
        int typeToSpawn = 0;
        float progress = (float)enemiesSpawnedInWave / enemiesToSpawnThisWave;

        if (currentTheme == 0) {
            if (currentLevel >= 3 && progress > 0.70f) typeToSpawn = 2;
            else typeToSpawn = rand() % 2;
        }
        else if (currentTheme == 1) {
            if (progress < 0.60f) typeToSpawn = (rand() % 2);
            else if (progress < 0.90f) typeToSpawn = 2;
            else typeToSpawn = 3;
        }
        else if (currentTheme == 2) {
            if (progress < 0.50f) typeToSpawn = (rand() % 2 + 1);
            else if (progress < 0.85f) typeToSpawn = 3;
            else typeToSpawn = 5;
        }
        else {
            if (progress < 0.40f) typeToSpawn = 3;
            else if (progress < 0.80f) typeToSpawn = 2;
            else typeToSpawn = 5;
        }

        if (typeToSpawn == 0) enemies[enemyCount] = new BasicEnemy1(spawnPos);
        else if (typeToSpawn == 1) enemies[enemyCount] = new BasicEnemy2(spawnPos);
        else if (typeToSpawn == 2) enemies[enemyCount] = new FastEnemy(spawnPos);
        else if (typeToSpawn == 3) enemies[enemyCount] = new TankEnemy(spawnPos);
        else if (typeToSpawn == 5) enemies[enemyCount] = new MidBoss(spawnPos);
        else enemies[enemyCount] = new BasicEnemy1(spawnPos);

        enemyCount++;
    }

    void placeTower(int gx, int gy) {
        if (gx < 0 || gx >= MAP_WIDTH || gy < 0 || gy >= MAP_HEIGHT) return;

        for (int i = 0; i < towerCount; i++) {
            sf::Vector2f pos = towers[i]->getSprite().getPosition();
            int tx = static_cast<int>(pos.x) / TILE_SIZE;
            int ty = (static_cast<int>(pos.y) - 400) / TILE_SIZE;
            if (tx == gx && ty == gy) return;
        }

        int cost = towerPrices[selectedTowerType];
        if (myMap.grid[gy][gx] == 2 && towerCount < MAX_TOWERS) {
            if (playerMoney >= cost) {
                sf::Vector2f targetPos(gx * TILE_SIZE, (gy * TILE_SIZE) + 400);
                Tower* temp = nullptr;

                if (selectedTowerType == 0) temp = new Cannon(targetPos);
                else if (selectedTowerType == 1) temp = new MachineGun(targetPos);
                else if (selectedTowerType == 2) temp = new Sniper(targetPos);
                else if (selectedTowerType == 3) temp = new FreezeTower(targetPos);
                else if (selectedTowerType == 4) temp = new Flamethrower(targetPos);

                if (temp) {
                    towers[towerCount] = temp;
                    towerCount++;
                    playerMoney -= cost;
                }
            }
        }
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {
                if (!isWaveActive) {
                    prepareWave();
                    isWaveActive = true;
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

                // 1. CHECK UI CLICKS FIRST (Top Right & Sidebar)
                // Default View check for Pause/Speed
                window.setView(window.getDefaultView());
                sf::Vector2f mouseMain = window.mapPixelToCoords(pixelPos);

                if (pauseBtn.getGlobalBounds().contains(mouseMain)) {
                    isPaused = !isPaused;
                    return;
                }
                if (speed2xBtn.getGlobalBounds().contains(mouseMain)) {
                    gameSpeed = (gameSpeed == 1.0f) ? 2.0f : 1.0f;
                    return;
                }

                // UI Sidebar View check for Upgrades/Abilities
                sf::View uiView(sf::FloatRect(0, 0, 320, 720));
                uiView.setViewport(sf::FloatRect(0.78f, 0, 0.22f, 1.0f));
                sf::Vector2f mouseUI = window.mapPixelToCoords(pixelPos, uiView);

                if (selectedTower) {
                    if (upgradeDmgBtn.getGlobalBounds().contains(mouseUI) && playerMoney >= 100) {
                        selectedTower->upgradeDamage(5);
                        playerMoney -= 100;
                    }
                    if (upgradeSpdBtn.getGlobalBounds().contains(mouseUI) && playerMoney >= 100) {
                        selectedTower->upgradeSpeed(0.5);
                        playerMoney -= 100;
                    }
                }

                if (nukeBtn.getGlobalBounds().contains(mouseUI) && playerMoney >= 500) {
                    for (int i = 0; i < enemyCount; i++) enemies[i]->health -= 100;
                    playerMoney -= 500;
                }

                // 2. CHECK GAME WORLD CLICKS
                sf::View gameView(sf::FloatRect(0, 400, 960, 400));
                gameView.setViewport(sf::FloatRect(0, 0.38f, 0.78f, 0.5f));
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, gameView);

                if (isRadialMenuOpen) {
                    bool clickedOption = false;
                    for (int i = 0; i < 5; i++) {
                        float angle = i * (2 * 3.14159f / 5.0f);
                        sf::Vector2f iconPos(menuCenter.x + cos(angle) * radialRadius, menuCenter.y + sin(angle) * radialRadius);
                        float dx = worldPos.x - iconPos.x;
                        float dy = worldPos.y - iconPos.y;
                        if (sqrt(dx * dx + dy * dy) < 30.0f) {
                            selectedTowerType = i;
                            placeTower(activeGridPos.x, activeGridPos.y);
                            isRadialMenuOpen = false;
                            clickedOption = true;
                            break;
                        }
                    }
                    if (!clickedOption) isRadialMenuOpen = false;
                }
                else {
                    if (worldPos.y < 400) continue;

                    int gx = static_cast<int>(worldPos.x) / TILE_SIZE;
                    int gy = (static_cast<int>(worldPos.y) - 400) / TILE_SIZE;

                    if (gx >= 0 && gx < MAP_WIDTH && gy >= 0 && gy < MAP_HEIGHT) {
                        bool towerClicked = false;
                        selectedTower = nullptr; // Reset selection
                        for (int i = 0; i < towerCount; i++) {
                            if (towers[i]->getSprite().getGlobalBounds().contains(worldPos)) {
                                towers[i]->isSelected = true;
                                selectedTower = towers[i]; // Store current tower for upgrade UI
                                towerClicked = true;
                            }
                            else towers[i]->isSelected = false;
                        }

                        if (!towerClicked && myMap.grid[gy][gx] == 2) {
                            isRadialMenuOpen = true;
                            menuScale = 0.0f;
                            activeGridPos = sf::Vector2i(gx, gy);
                            menuCenter = sf::Vector2f(gx * TILE_SIZE + 32, (gy * TILE_SIZE + 400) + 32);
                        }
                    }
                }
            }
        }
    }

    void update(float dt) {
        if (playState != PlayState::Playing || isPaused) return;

        if (playerLives <= 0) {
            playerLives = 0;
            playState = PlayState::Lost;
        }

        if (isRadialMenuOpen) {
            if (menuScale < 1.0f) menuScale += dt * 7.0f;
            if (menuScale > 1.0f) menuScale = 1.0f;
        }

        if (isWaveActive) {
            spawnTimer += dt;
            if (enemiesSpawnedInWave < enemiesToSpawnThisWave) {
                if (spawnTimer >= timeBetweenSpawns) {
                    spawnEnemyLogic();
                    enemiesSpawnedInWave++;
                    spawnTimer = 0.0f;
                }
            }
            else if (enemyCount == 0) {
                isWaveActive = false;
                currentWave++;
                playerMoney += (50 + currentWave * 10);
                if (currentWave >= totalWavesInLevel) nextLevel();
            }
        }

        for (int i = 0; i < towerCount; i++) {
            if (towers[i]) towers[i]->update(dt, enemies, enemyCount);
        }

        for (int i = 0; i < enemyCount; i++) {
            if (enemies[i]) {
                enemies[i]->update(dt);
                enemies[i]->moveOnPath(myMap.waypoints, myMap.waypointCount, dt);

                if (enemies[i]->isDead || enemies[i]->isFinished) {
                    if (enemies[i]->isFinished) playerLives--;
                    else playerMoney += enemies[i]->getReward();

                    delete enemies[i];
                    
                    for (int j = i; j < enemyCount - 1; j++) enemies[j] = enemies[j + 1];
                    enemies[--enemyCount] = nullptr;
                    i--;
                }
            }
        }
    }

    void nextLevel() {
        for (int i = 0; i < MAX_ENEMIES; i++) { delete enemies[i]; enemies[i] = nullptr; }
        for (int i = 0; i < MAX_TOWERS; i++) { delete towers[i]; towers[i] = nullptr; }
        enemyCount = 0; towerCount = 0; selectedTower = nullptr;

        currentLevel++;
        if (currentLevel > totalLevels) { playState = PlayState::Won; return; }

        currentTheme = (currentLevel - 1) / 5;
        myMap.loadLevel(currentLevel);
        currentWave = 0;
        playerLives += 2;
    }

    void renderHUD() {
        sf::View uiView(sf::FloatRect(0, 0, 320, 720));
        uiView.setViewport(sf::FloatRect(0.78f, 0, 0.22f, 1.0f));
        window.setView(uiView);

        sf::RectangleShape panel(sf::Vector2f(320, 720));
        panel.setFillColor(sf::Color(25, 25, 25));
        panel.setOutlineThickness(-2);
        panel.setOutlineColor(sf::Color(100, 100, 100));
        window.draw(panel);

        sf::Text header;
        header.setFont(font);
        header.setString("LEVEL " + std::to_string(currentLevel) + " - W" + std::to_string(currentWave + 1));
        header.setCharacterSize(26);
        header.setPosition(20, 30);
        window.draw(header);

        drawStat(20, 100, "GOLD:", std::to_string(playerMoney), sf::Color::Yellow);
        drawStat(20, 140, "LIVES:", std::to_string(playerLives), sf::Color::Red);

        // ABILITY BUTTONS
        nukeBtn.setSize(sf::Vector2f(280, 50));
        nukeBtn.setPosition(20, 550);
        nukeBtn.setFillColor(sf::Color(150, 50, 50));
        window.draw(nukeBtn);
        drawStat(40, 560, "NUKE", "500G", sf::Color::White);

        // TOWER UPGRADE UI
        if (selectedTower) {
            sf::Text upText; upText.setFont(font); upText.setCharacterSize(22);
            upText.setString("TOWER UPGRADES"); upText.setPosition(20, 250);
            window.draw(upText);

            upgradeDmgBtn.setSize(sf::Vector2f(280, 45));
            upgradeDmgBtn.setPosition(20, 300);
            upgradeDmgBtn.setFillColor(sf::Color(50, 50, 150));
            window.draw(upgradeDmgBtn);
            drawStat(40, 310, "DAMAGE +", "100G", sf::Color::Cyan);

            upgradeSpdBtn.setSize(sf::Vector2f(280, 45));
            upgradeSpdBtn.setPosition(20, 360);
            upgradeSpdBtn.setFillColor(sf::Color(50, 150, 50));
            window.draw(upgradeSpdBtn);
            drawStat(40, 370, "SPEED +", "100G", sf::Color::Cyan);
        }

        // TOP RIGHT BUTTONS (Switch to Default View for top right positioning)
        window.setView(window.getDefaultView());
        pauseBtn.setSize(sf::Vector2f(50, 50));
        pauseBtn.setPosition(960 - 120, 20);
        pauseBtn.setFillColor(isPaused ? sf::Color::Red : sf::Color::White);
        window.draw(pauseBtn);

        speed2xBtn.setSize(sf::Vector2f(50, 50));
        speed2xBtn.setPosition(960 - 60, 20);
        speed2xBtn.setFillColor(gameSpeed > 1.0f ? sf::Color::Cyan : sf::Color::White);
        window.draw(speed2xBtn);
    }

    void drawStat(float x, float y, std::string label, std::string value, sf::Color valColor) {
        sf::Text tLabel; tLabel.setFont(font); tLabel.setString(label);
        tLabel.setCharacterSize(20); tLabel.setPosition(x, y); window.draw(tLabel);

        sf::Text tVal; tVal.setFont(font); tVal.setString(value);
        tVal.setCharacterSize(22); tVal.setFillColor(valColor);
        tVal.setPosition(x + 100, y - 2); window.draw(tVal);
    }

    void renderEndScreen() {
        sf::RectangleShape dim(sf::Vector2f(1280, 720));
        dim.setFillColor(sf::Color(0, 0, 0, 180));
        window.setView(window.getDefaultView());
        window.draw(dim);

        sf::Text endText;
        endText.setFont(font);
        endText.setCharacterSize(80);
        endText.setString(playState == PlayState::Lost ? "DEFEAT" : "VICTORY");
        endText.setFillColor(playState == PlayState::Lost ? sf::Color::Red : sf::Color::Cyan);

        sf::FloatRect textRect = endText.getLocalBounds();
        endText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        endText.setPosition(1280 / 2.0f, 720 / 2.0f - 50);
        window.draw(endText);
    }

    void render() {
        window.clear(sf::Color(30, 30, 30));

        // Background / Theme View
        sf::View topView(sf::FloatRect(0, 0, 960, 720));
        topView.setViewport(sf::FloatRect(0.0f, 0.0f, 0.78f, 1.0f));
        window.setView(topView);
        window.draw(sf::Sprite(themeBG[currentTheme]));

        // Game Grid View
        sf::View gameView(sf::FloatRect(0, 400, 960, 400));
        gameView.setViewport(sf::FloatRect(0, 0.38f, 0.78f, 0.5f));
        window.setView(gameView);

        myMap.draw(window, currentTheme);

        for (int i = 0; i < towerCount; i++) {
            if (towers[i]) towers[i]->draw(window);
        }

        for (int i = 0; i < enemyCount; i++) {
            if (enemies[i]) {
                enemies[i]->draw(window);
                enemies[i]->drawHealthBar(window);
            }
        }

        if (isRadialMenuOpen) {
            sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), gameView);
            for (int i = 0; i < 5; i++) {
                float angle = i * (2 * 3.14159f / 5.0f);
                float currentRadius = radialRadius * menuScale;
                sf::Vector2f iconPos(menuCenter.x + cos(angle) * currentRadius, menuCenter.y + sin(angle) * currentRadius);
                int cost = towerPrices[i];
                bool canAfford = (playerMoney >= cost);

                sf::CircleShape icon(28 * menuScale);
                icon.setOrigin(icon.getRadius(), icon.getRadius());
                icon.setPosition(iconPos);
                icon.setOutlineThickness(2);

                float dx = mPos.x - iconPos.x;
                float dy = mPos.y - iconPos.y;
                bool isHovered = (sqrt(dx * dx + dy * dy) < 28.0f * menuScale && menuScale >= 1.0f);

                if (isHovered && menuScale >= 1.0f) {
                    float baseRange = towerRanges[i];
                    sf::CircleShape hoveredRange(baseRange);
                    hoveredRange.setOrigin(baseRange, baseRange);
                    hoveredRange.setPosition(menuCenter);
                    hoveredRange.setScale(1.0f, 0.7f); // Perspective scale
                    hoveredRange.setFillColor(canAfford ? sf::Color(0, 255, 0, 40) : sf::Color(255, 0, 0, 30));
                    hoveredRange.setOutlineColor(canAfford ? sf::Color(0, 255, 0, 100) : sf::Color(255, 0, 0, 80));
                    hoveredRange.setOutlineThickness(2.0f);
                    window.draw(hoveredRange);
                }

                icon.setFillColor(!canAfford ? sf::Color(80, 20, 20, 200) : (isHovered ? sf::Color(100, 100, 100) : sf::Color(60, 60, 60, 220)));
                icon.setOutlineColor(!canAfford ? sf::Color::Red : (isHovered ? sf::Color::Yellow : sf::Color::White));
                if (isHovered && canAfford) icon.setScale(1.2f, 1.2f);
                window.draw(icon);

                if (menuScale > 0.05f && towerIcons[i].getSize().x > 0) {
                    sf::Sprite iconSprite(towerIcons[i]);
                    sf::Vector2u texSize = towerIcons[i].getSize();
                    iconSprite.setOrigin(texSize.x / 2.0f, texSize.y / 2.0f);
                    iconSprite.setPosition(iconPos);
                    float finalScale = (44.0f / texSize.x) * menuScale;
                    if (isHovered && canAfford) finalScale *= 1.2f;
                    iconSprite.setScale(finalScale, finalScale);
                    iconSprite.setColor(canAfford ? sf::Color::White : sf::Color(40, 40, 40, 160));
                    window.draw(iconSprite);
                }
            }
        }

        renderHUD();
        if (playState != PlayState::Playing) renderEndScreen();
        window.display();
    }

public:
    GameEngine(sf::RenderWindow& win, GameState selectedMode) : window(win), mode(selectedMode) {
        srand(static_cast<unsigned>(time(0)));
        isEndlessMode = (selectedMode == GameState::EndlessMode);
        for (int i = 0; i < MAX_ENEMIES; i++) enemies[i] = nullptr;
        for (int i = 0; i < MAX_TOWERS; i++) towers[i] = nullptr;

        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) font.loadFromFile("C:/Windows/Fonts/consola.ttf");

        loadAllAssets();
        myMap.loadLevel(1);
    }

    ~GameEngine() {
        for (int i = 0; i < MAX_ENEMIES; i++) delete enemies[i];
        for (int i = 0; i < MAX_TOWERS; i++) delete towers[i];
    }

    void run() {
        sf::Clock clock;
        while (window.isOpen()) {
            float dt = clock.restart().asSeconds() * (isPaused ? 0.0f : gameSpeed);
            handleEvents();
            update(dt);
            render();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.setView(window.getDefaultView());
                return;
            }
        }
    }
};