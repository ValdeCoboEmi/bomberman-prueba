#ifndef HUD_H
#define HUD_H

#include "Player.h"

class HUD
{
public:
    int getHeight() const;
    int getWidth() const;

    void draw(const Player &player, int currentLevel, int mapWidth = 0);

};

#endif
