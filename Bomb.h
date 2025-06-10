#ifndef BOMB_H
#define BOMB_H

#include <chrono> // Biblioteca para manejar el tiempo (reloj, duración, etc.)

// Clase que representa una bomba en el juego
class Bomb
{
public:
    // Constructor por defecto necesario para arreglos estáticos o sin inicialización directa
    Bomb() : x(0), y(0), placedAt(std::chrono::steady_clock::now()) {}

    // Constructor personalizado: inicializa la bomba con su posición (x, y) y guarda el tiempo en que fue colocada
    Bomb(int x, int y)
        : x(x), y(y), placedAt(std::chrono::steady_clock::now()) {}

    // Devuelve la posición X de la bomba
    int getX() const { return x; }

    // Devuelve la posición Y de la bomba
    int getY() const { return y; }

    // Verifica si la bomba ha explotado (han pasado 3 segundos desde que fue colocada)
    bool hasExploded() const
    {
        return std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::steady_clock::now() - placedAt)
                    .count() >= 3;
    }

    // Método de actualización (actualmente no hace nada, pero se puede usar en el futuro)
    void update() {}

private:
    int x, y;                                       // Posición de la bomba
    std::chrono::steady_clock::time_point placedAt; // Momento en que se colocó la bomba
};

#endif // BOMB_H
