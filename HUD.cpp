// HUD.cpp
#include "HUD.h"
#include "Utils.h"
#include <iostream>

int HUD::getHeight() const {
    return 95; // ahora ocupa toda la altura de la ventana
}

int HUD::getWidth() const {
    return 25; // ancho fijo de 30 caracteres
}

void HUD::draw(const Player &player, int currentLevel, int mapWidth) {
    int hudX = mapWidth + 3; // coloca el HUD 5 espacios a la derecha del mapa
    int y = 2;

    Utils::moveCursor(hudX, y++);
    std::cout << "Sala - " << currentLevel;

    Utils::moveCursor(hudX, y++);
    std::cout << "Vida - " << player.getLives();

    Utils::moveCursor(hudX, y++);
    std::cout << "0 - " << player.getBombs();

    Utils::moveCursor(hudX, y++);
    std::cout << "[Objetivo]:";

    Utils::moveCursor(hudX, y++);
    std::cout << "Encuentra la puerta";

    Utils::moveCursor(hudX, y++);
    std::cout << "Destruye % con bombas";

    Utils::moveCursor(hudX, y++);
    std::cout << "Evita morir con fuego";

    Utils::moveCursor(hudX, y++);
    std::cout << "Controles:";

    Utils::moveCursor(hudX, y++);
    std::cout << "[WASD]: Moverse";

    Utils::moveCursor(hudX, y++);
    std::cout << "[B]: Bomba";
}
