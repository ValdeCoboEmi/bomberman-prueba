#ifndef MAP_H
#define MAP_H

#include <string>
#include <fstream>
#include <iostream>
#include "Bomb.h"
#include "Utils.h"
#include "Colors.h"

const int MAX_HEIGHT = 95;
const int MAX_WIDTH = 60;
const int MAX_BOMBS = 25; 

class Map {
public:
    Map();
    bool loadFromFile(const std::string &filename);
    void draw(int playerX, int playerY, Bomb bombs[], int bombCount, int offsetX = 0, int offsetY = 0);
    char getTile(int x, int y) const;
    void setTile(int x, int y, char value);
    int getWidth() const;
    int getHeight() const;
    int getSpawnX() const { return spawnX; }
    int getSpawnY() const { return spawnY; }

private:
    char grid[MAX_HEIGHT][MAX_WIDTH]{};
    int width = 0;
    int height = 0;
    int spawnX = 1;
    int spawnY = 1;
};

Map::Map() {}

bool Map::loadFromFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;
    int y = 0;
    while (std::getline(file, line) && y < MAX_HEIGHT) {
        int lineLength = std::min((int)line.size(), MAX_WIDTH);
        for (int x = 0; x < lineLength; ++x) {
            grid[y][x] = line[x];
            if (line[x] == ']') {
                spawnX = x + 1;
                spawnY = y;
            }
        }
        for (int x = lineLength; x < MAX_WIDTH; ++x) {
            grid[y][x] = ' ';
        }
        ++y;
    }

    height = y;
    width = MAX_WIDTH;
    return true;
}

void Map::draw(int playerX, int playerY, Bomb bombs[], int bombCount, int offsetX, int offsetY) {
    for (int y = 0; y < getHeight(); ++y) {
        for (int x = 0; x < getWidth(); ++x) {
            Utils::moveCursor(offsetX + x, offsetY + y);
            if (x == playerX && y == playerY) {
                std::cout << PINK << "o" << RESET;
            } else {
                bool isBomb = false;
                for (int i = 0; i < bombCount; ++i) {
                    if (bombs[i].getX() == x && bombs[i].getY() == y) {
                        isBomb = true;
                        break;
                    }
                }

                if (isBomb) {
                    std::cout << ORANGE << "0" << RESET;
                } else {
                    char tile = grid[y][x];
                    switch (tile) {
                        case '#': std::cout << GRAY << "#" << RESET; break;
                        case ']': std::cout << GRAY << "]" << RESET; break;
                        case 'B': std::cout << ORANGE << "B" << RESET; break;
                        case 'A': std::cout << GREEN << "#" << RESET; break;
                        case 'H': std::cout << GREEN << "O" << RESET; break;
                        case 'T': std::cout << BROWN << "W" << RESET; break;
                        case '~': std::cout << BLUE << "~" << RESET; break;
                        case '*': std::cout << YELLOW_BRIGHT << "*" << RESET; break;
                        default: std::cout << tile; break;
                    }
                }
            }
        }
    }
}

char Map::getTile(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return grid[y][x];
    }
    return ' ';
}

void Map::setTile(int x, int y, char value) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x] = value;
    }
}

int Map::getWidth() const { return width; }
int Map::getHeight() const { return height; }

#endif // MAP_H
