#ifndef HUD_H
#define HUD_H

#include "Player.h"
#include "Colors.h"
#include <string>

class HUD
{
public:
    int getHeight() const;
    int getWidth() const;
    void draw(const Player &player, int currentLevel, int mapWidth = 0);

private:
    void printLine(int x, int &y, const std::string &text, const std::string &color = GRAY_BRIGHT);
};

#endif
