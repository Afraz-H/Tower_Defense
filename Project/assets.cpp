#include "assets.h"
#include <iostream>
#include <string>

// Defining the memory
sf::Texture towerTex[6]; //1
sf::Texture towerIcons[6]; //2
sf::Texture enemyTex[7]; //3
sf::Texture tileTex[5][10]; //4
sf::Texture themeBG[5]; //5
sf::Texture effects[5]; //6

void loadAllAssets() {
    // 1. Load Towers
    for (int i = 0; i < 6; i++) {
        if (!towerTex[i].loadFromFile("assets/tower" + std::to_string(i) + ".png"))
            std::cout << "Error: Tower " << i << " missing!" << std::endl;
    }

    // 2. Load Enemies
    for (int i = 0; i < 7; i++) {
        if (!enemyTex[i].loadFromFile("assets/enemy" + std::to_string(i) + ".png"))
            std::cout << "Error: Enemy " << i << " missing!" << std::endl;
    }

    // 3. Load Themes (Backgrounds and Tiles)
    for (int t = 0; t < 5; t++) {
        // Load the 50% Static Background for this theme
        themeBG[t].loadFromFile("assets/bg_theme" + std::to_string(t) + ".png");

        // Load the tiles (0-9) for this theme
        for (int i = 0; i < 10; i++) {
            std::string path = "assets/t" + std::to_string(t) + "_tile" + std::to_string(i) + ".png";
            tileTex[t][i].loadFromFile(path);
        }
    }

    // 4. Load effects
    for (int i = 0; i < 5; i++)
    {
        if (!effects[i].loadFromFile("assets/effect" + std::to_string(i) + ".png"))
            std::cout << "Error: Effect " << i << " missing!" << std::endl;
    }

    // 5. Load Icons
    for (int i = 0; i < 6; i++)
    {
        if (!towerIcons[i].loadFromFile("assets/icon" + std::to_string(i) + ".png"))
            std::cout << "Error: Tower Icon " << i << " missing!" << std::endl;
    }

}