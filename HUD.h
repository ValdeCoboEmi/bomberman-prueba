#ifndef HUD_H
#define HUD_H

#include "Player.h"

class HUD
{
public:
    int getHeight() const;

    void draw(const Player &player, int currentLevel, int mapWidth = 0);

};

#endif
