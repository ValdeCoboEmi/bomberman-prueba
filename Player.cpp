#include "Player.h"

Player::Player() : x(1), y(1), lives(3), bombsAvailable(5), maxBombs(25) {}

int Player::getX() const { return x; }
int Player::getY() const { return y; }
int Player::getLives() const { return lives; }
int Player::getBombs() const { return bombsAvailable; }
int Player::getMaxBombs() const { return maxBombs; }

void Player::move(int dx, int dy, char nextTile) {
    if (nextTile == ' ' || nextTile == '/') {
        x += dx;
        y += dy;
    }
}

bool Player::canPlaceBomb() const {
    return bombsAvailable > 0;
}

void Player::placeBomb() {
    if (bombsAvailable > 0) {
        --bombsAvailable;
    }
}

void Player::restoreBomb() {
    if (bombsAvailable < maxBombs)
        ++bombsAvailable;
}

void Player::addMaxBomb() {
    ++maxBombs;
    ++bombsAvailable;  
}

void Player::loseLife() {
    --lives;
}

void Player::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}
