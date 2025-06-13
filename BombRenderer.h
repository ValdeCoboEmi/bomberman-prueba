#ifndef BOMB_RENDERER_H
#define BOMB_RENDERER_H

#include <iostream>
#include "Bomb.h"
#include "Utils.h"
#include "Colors.h"

class BombRenderer {
public:
    static void draw(Bomb bombs[], int count, int offsetX = 0, int offsetY = 0) {
        for (int i = 0; i < count; ++i) {
            int x = bombs[i].GetX();
            int y = bombs[i].GetY();
            Utils::moveCursor(offsetX + x, offsetY + y);
            std::cout << ORANGE << "0" << RESET;
        }
    }
};

#endif
