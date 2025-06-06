#ifndef BOMB_H
#define BOMB_H

#include <chrono>

class Bomb {
public:
    Bomb(int x, int y);

    int getX() const;
    int getY() const;
    bool hasExploded() const;
    void update();

private:
    int x, y;
    std::chrono::steady_clock::time_point placedAt;
};

#endif
