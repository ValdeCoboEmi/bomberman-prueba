#ifndef HUD_H
#define HUD_H

#include "Player.h"
#include "Colors.h"
#include "Utils.h"
#include <string>
#include <iostream>

class HUD
{
public:
    int getHeight() const;
    int getWidth() const;
    void draw(const Player &player, int currentLevel, int mapWidth = 0);

private:
    void printLine(int x, int &y, const std::string &text, const std::string &color = GRAY_BRIGHT);
};

// Implementación

int HUD::getHeight() const
{
    return 95; // HUD ocupa toda la altura de la ventana
}

int HUD::getWidth() const
{
    return 25; // Ancho fijo del HUD
}

// Función auxiliar para imprimir una línea de HUD
void HUD::printLine(int x, int &y, const std::string &text, const std::string &color)
{
    std::cout << "\033[" << y << ";" << x << "H" << color << text << RESET << std::endl;
    y++;
}

void HUD::draw(const Player &player, int currentLevel, int mapWidth)
{
    int hudX = mapWidth + 3; // HUD se dibuja justo a la derecha del mapa
    int y = 2;

    // --- Sección: Información del jugador ---
    printLine(hudX, y, "Sala - " + std::to_string(currentLevel), WHITE_BRIGHT);
    y++; // Línea vacía

    printLine(hudX, y, "Vida - " + std::to_string(player.getLives()), RED);
    // Borrar la línea actual antes de escribir las bombas
    std::cout << "\033[" << y << ";" << hudX << "H" << std::string(25, ' '); // Borra la línea completa (25 espacios)
    printLine(hudX, y, "B - " + std::to_string(player.getBombs()), ORANGE);
    y++; // Línea vacía

    // --- Sección: Objetivo del juego ---
    printLine(hudX, y, "[Objetivo]:", WHITE_BRIGHT);
    printLine(hudX, y, "Encuentra la puerta");
    printLine(hudX, y, "Destruye % con bombas");
    printLine(hudX, y, "Evita morir con fuego");
    y++; // Línea vacía

    // --- Sección: Controles ---
    printLine(hudX, y, "Controles:", WHITE_BRIGHT);
    printLine(hudX, y, "[WASD]: Moverse");
    printLine(hudX, y, "[B]: Bomba");

    std::cout << RESET; // Restablecer color
}

#endif // HUD_H
