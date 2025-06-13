#ifndef BOMB_H
#define BOMB_H

#include <chrono>

class Bomb
{
public:
    Bomb();
    Bomb(int x, int y);

    int GetX() const;
    int GetY() const;
    bool HasExploded() const;

private:
    int x, y;
    std::chrono::steady_clock::time_point placedAt;
};

// Implementación de Bomb
// Constructor por defecto y con coordenadas
Bomb::Bomb() : x(0), y(0), placedAt(std::chrono::steady_clock::now()) {}
Bomb::Bomb(int x, int y) : x(x), y(y), placedAt(std::chrono::steady_clock::now()) {}

// Getters para las coordenadas
int Bomb::GetX() const { return x; }
int Bomb::GetY() const { return y; }

// Verifica si la bomba ha explotado (3 segundos después de ser colocada)
bool Bomb::HasExploded() const {
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - placedAt).count() >= 3;
}

#endif 
