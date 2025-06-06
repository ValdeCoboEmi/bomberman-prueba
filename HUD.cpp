// HUD.cpp
#include "HUD.h"
#include "Utils.h"
#include <iostream>
#include "Colors.h"

int HUD::getHeight() const
{
    return 95; // ahora ocupa toda la altura de la ventana
}

int HUD::getWidth() const
{
    return 25; // ancho fijo de 30 caracteres
}

void HUD::draw(const Player &player, int currentLevel, int mapWidth)
{
    int hudX = mapWidth + 3; // coloca el HUD 5 espacios a la derecha del mapa
    int y = 2;

    Utils::moveCursor(hudX, y++);
    std::cout << WHITE_BRIGHT << "Sala" << GRAY_BRIGHT << " - " << currentLevel;

    Utils::moveCursor(hudX, y++);
    std::cout << "\n";
    Utils::moveCursor(hudX, y++);
    std::cout << "\n";

    Utils::moveCursor(hudX, y++);
    std::cout << RED << "Vida" << GRAY_BRIGHT << " - " << player.getLives();

    Utils::moveCursor(hudX, y++);
    std::cout << ORANGE << "B" << GRAY_BRIGHT << " - " <<  player.getBombs();

    Utils::moveCursor(hudX, y++);
    std::cout << "\n";

    Utils::moveCursor(hudX, y++);
    std::cout << WHITE_BRIGHT << "[Objetivo]:" << GRAY_BRIGHT;

    Utils::moveCursor(hudX, y++);
    std::cout << "Encuentra la puerta";

    Utils::moveCursor(hudX, y++);
    std::cout << "Destruye % con bombas";

    Utils::moveCursor(hudX, y++);
    std::cout << "Evita morir con fuego";

    Utils::moveCursor(hudX, y++);
    std::cout << "\n";

    Utils::moveCursor(hudX, y++);
    std::cout << WHITE_BRIGHT << "Controles:" << GRAY_BRIGHT;

    Utils::moveCursor(hudX, y++);
    std::cout << "[WASD]: Moverse";

    Utils::moveCursor(hudX, y++);
    std::cout << "[B]: Bomba" << RESET;
}
