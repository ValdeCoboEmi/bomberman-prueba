#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include "Bomb.h"

class Map
{
public:
    Map();
    bool loadFromFile(const std::string &filename);
    void draw(int playerX, int playerY, const std::vector<Bomb> &bombs, int offsetX = 0, int offsetY = 0);
    char getTile(int x, int y) const;
    void setTile(int x, int y, char value);
    int getWidth() const;
    int getHeight() const;
    int getSpawnX() const { return spawnX; }
    int getSpawnY() const { return spawnY; }

private:
    std::vector<std::string> grid;
    int spawnX = 1;
    int spawnY = 1;
};

#endif
