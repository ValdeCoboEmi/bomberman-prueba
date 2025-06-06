#include "Bomb.h"
#include <chrono>

using namespace std::chrono;

Bomb::Bomb(int x, int y) : x(x), y(y), placedAt(steady_clock::now()) {}

int Bomb::getX() const { return x; }
int Bomb::getY() const { return y; }

bool Bomb::hasExploded() const {
    return duration_cast<seconds>(steady_clock::now() - placedAt).count() >= 3;
}

void Bomb::update() {
    // Aquí podrías expandir lógica visual futura
}
