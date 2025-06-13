#ifndef HUD_BASE_H
#define HUD_BASE_H

#include "Player.h"

class HUDBase {
public:
    virtual ~HUDBase() = default;
    virtual int GetWidth() const = 0;
    virtual void Draw(const Player& player, int currentLevel, int mapWidth = 0) = 0;
};

#endif // HUD_BASE_H
