#ifndef ASSETS_H
#define ASSETS_H

#include <SFML/Graphics.hpp>

// --- TEXTURE ARRAYS ---
extern sf::Texture towerTex[6];
extern sf::Texture towerIcons[6];
extern sf::Texture enemyTex[7];
extern sf::Texture tileTex[5][10]; // 4 Themes, 10 types each
extern sf::Texture themeBG[5];
extern sf::Texture effects[5];

// --- FUNCTION TO LOAD EVERYTHING ---
void loadAllAssets();

#endif