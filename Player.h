#ifndef PLAYER_H
#define PLAYER_H

class Player {
public:
    Player();

    int getX() const;
    int getY() const;
    int getLives() const;
    int getBombs() const;
    int getMaxBombs() const;
    bool canPlaceBomb() const;

    void move(int dx, int dy, char nextTile);
    void placeBomb();
    void loseLife();
    void restoreBomb();      // cuando una bomba explota
    void addMaxBomb();       // cuando recojo una B
    void setPosition(int x, int y);

private:
    int x, y;
    int lives;
    int bombsAvailable;  // Cuántas bombas puedo colocar ahora
    int maxBombs;        // Cuántas bombas puedo tener máximo
};

// Implementación

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

#endif // PLAYER_H
