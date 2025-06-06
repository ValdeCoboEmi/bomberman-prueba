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

#endif
