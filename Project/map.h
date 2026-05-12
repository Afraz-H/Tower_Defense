#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include "assets.h"

const int TILE_SIZE = 64;
const int MAP_WIDTH = 15;  // 1000px / 40 (leaves space for UI)
const int MAP_HEIGHT = 5;  // 360px / 40 (bottom half of screen)

class Map {
public:
    int grid[MAP_HEIGHT][MAP_WIDTH];
    sf::Vector2f path[100];
    int pathCount = 0;
    sf::Vector2f waypoints[100];
    int waypointCount = 0;

    void loadLevel(int levelNum) {
        // Construct filename: "levels/level1.txt"
        std::string filename = "levels/level" + std::to_string(levelNum) + ".txt";
        std::ifstream file(filename);
        pathCount = 0;

        if (!file.is_open()) return;

        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
                if (!(file >> grid[i][j])) grid[i][j] = 0;

                // If tile is path (1), start (2), or exit (3), add to path points
                if (grid[i][j] >= 1 && grid[i][j] <= 3) {
                    // Y starts at 360 because the top half is the static image
                    path[pathCount] = sf::Vector2f(j * TILE_SIZE, (i * TILE_SIZE) + 360);
                    pathCount++;
                }
            }
        }
        file.close();

        generateWaypoints();

        std::cout << "Path generated with " << waypointCount << " points." << std::endl;
    }

    void draw(sf::RenderWindow& window, int theme) {
        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
                int type = grid[i][j];
                if (type > 0) {
                    sf::Sprite tile;
                    tile.setTexture(tileTex[theme][type]);

                    // FORCE SCALE: Ensure tile fits exactly TILE_SIZE (40px)
                    float sX = (float)TILE_SIZE / tileTex[theme][type].getSize().x;
                    float sY = (float)TILE_SIZE / tileTex[theme][type].getSize().y;
                    tile.setScale(sX, sY);

                    tile.setPosition(j * TILE_SIZE, (i * TILE_SIZE) + 400); // Note: 400 matches your View
                    window.draw(tile);
                }
            }
        }
    }
    void generateWaypoints() {
        waypointCount = 0;
        sf::Vector2i currentPos(-1, -1);

        // 1. Find the starting tile (type 3)
        for (int y = 0; y < 5; y++) {
            for (int x = 0; x < 15; x++) {
                if (grid[y][x] == 3) {
                    currentPos = sf::Vector2i(x, y);
                    waypoints[waypointCount++] = sf::Vector2f(x * 64, y * 64 + 400);
                    break;
                }
            }
            if (currentPos.x != -1) break;
        }

        // 2. Follow the path of 1s
        sf::Vector2i lastPos = currentPos;
        bool pathEnded = false;

        while (!pathEnded && waypointCount < 20) {
            sf::Vector2i neighbors[] = {
                {currentPos.x + 1, currentPos.y}, // Right
                {currentPos.x - 1, currentPos.y}, // Left
                {currentPos.x, currentPos.y + 1}, // Down
                {currentPos.x, currentPos.y - 1}  // Up
            };

            bool foundNext = false;
            for (auto& next : neighbors) {
                // Check bounds
                if (next.x >= 0 && next.x < 15 && next.y >= 0 && next.y < 5) {
                    // Don't move backward to the tile we just came from
                    if (next == lastPos) continue;

                    int tile = grid[next.y][next.x];
                    // If it's a path (1) or the end point (3/6)
                    if (tile == 1 || tile == 3 || tile == 6) {
                        lastPos = currentPos;
                        currentPos = next;

                        // Add a waypoint if it's a corner or an endpoint
                        // (For simplicity, we add every path tile as a waypoint here)
                        waypoints[waypointCount++] = sf::Vector2f(next.x * 64, next.y * 64 + 400);

                        if (tile == 3 && waypointCount > 1) pathEnded = true; // Found the exit 3
                        if (tile == 6) pathEnded = true; // Found exit 6

                        foundNext = true;
                        break;
                    }
                }
            }
            if (!foundNext) pathEnded = true; // Dead end
        }
    }
};