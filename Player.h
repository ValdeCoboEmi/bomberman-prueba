#ifndef PLAYER_H
#define PLAYER_H

// Clase que representa al jugador en el juego
class Player {
public:
    Player();

    // Getters para obtener información del jugador
    int getX() const;              // Posición X del jugador en el mapa
    int getY() const;              // Posición Y del jugador en el mapa
    int getLives() const;          // Número de vidas actuales
    int getBombs() const;          // Bombas disponibles actualmente
    int getMaxBombs() const;       // Cantidad máxima de bombas que puede tener
    bool canPlaceBomb() const;     // Retorna true si tiene bombas disponibles para colocar

    // Acciones del jugador
    void move(int dx, int dy, char nextTile); // Mueve al jugador si la siguiente casilla es válida
    void placeBomb();                         // Coloca una bomba (reduce las bombas disponibles)
    void loseLife();                          // Resta una vida
    void restoreBomb();                       // Restaura una bomba (al explotar una bomba)
    void addMaxBomb();                        // Aumenta la cantidad máxima y actual de bombas (al recoger una 'B')
    void setPosition(int x, int y);           // Establece la posición del jugador (usado en reinicios o teletransportación)

private:
    // Atributos del jugador
    int x, y;               // Posición del jugador en el mapa
    int lives;              // Vidas actuales
    int bombsAvailable;     // Cuántas bombas puede colocar en este momento
    int maxBombs;           // Límite máximo de bombas que puede tener
};

// Implementación

// Constructor por defecto: posición inicial (1,1), 3 vidas, 5 bombas iniciales, máximo 25
Player::Player() : x(1), y(1), lives(3), bombsAvailable(5), maxBombs(25) {}

int Player::getX() const { return x; }
int Player::getY() const { return y; }
int Player::getLives() const { return lives; }
int Player::getBombs() const { return bombsAvailable; }
int Player::getMaxBombs() const { return maxBombs; }

// Mueve al jugador si la siguiente casilla es caminable (espacio vacío o puerta '/')
void Player::move(int dx, int dy, char nextTile) {
    if (nextTile == ' ' || nextTile == '/') {
        x += dx;
        y += dy;
    }
}

// Retorna si el jugador puede colocar una bomba
bool Player::canPlaceBomb() const {
    return bombsAvailable > 0;
}

// Coloca una bomba si tiene disponibles
void Player::placeBomb() {
    if (bombsAvailable > 0) {
        --bombsAvailable;
    }
}

// Restaura una bomba cuando una bomba que colocó explota
void Player::restoreBomb() {
    if (bombsAvailable < maxBombs)
        ++bombsAvailable;
}

// Aumenta la cantidad máxima de bombas y también la cantidad disponible
void Player::addMaxBomb() {
    ++maxBombs;
    ++bombsAvailable;
}

// Resta una vida al jugador
void Player::loseLife() {
    --lives;
}

// Establece manualmente la posición del jugador
void Player::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

#endif // PLAYER_H
