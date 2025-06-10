#ifndef BOMB_H
#define BOMB_H

#include <chrono>

class Bomb {
public:
    Bomb(int x, int y)
        : x(x), y(y), placedAt(std::chrono::steady_clock::now()) {}

    int getX() const { return x; }
    int getY() const { return y; }

    bool hasExploded() const {
        return std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::steady_clock::now() - placedAt)
                    .count() >= 3;
    }

    void update() {
    }

private:
    int x, y;
    std::chrono::steady_clock::time_point placedAt;
};

#endif // BOMB_H
