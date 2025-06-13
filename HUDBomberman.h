#ifndef HUD_BOMBERMAN_H
#define HUD_BOMBERMAN_H

#include "HUDBase.h"
#include "Colors.h"
#include "Utils.h"
#include <iostream>
#include <string>

class HUDBomberman : public HUDBase
{
public:
    int GetWidth() const override
    {
        return 25;
    }

    void Draw(const Player &player, int currentLevel, int mapWidth = 0) override
    {
        int hudX = mapWidth + 3;
        int y = 3;

        // Información del jugador
        PrintLine(hudX, y, "Sala - " + std::to_string(currentLevel), WHITE_BRIGHT);
        y++;

        PrintLine(hudX, y, "Vida - " + std::to_string(player.GetLives()), RED);
        std::cout << "\033[" << y << ";" << hudX << "H" << std::string(25, ' ');
        PrintLine(hudX, y, "B - " + std::to_string(player.GetBombs()), ORANGE);
        y++;

        // Objetivo del juego
        PrintLine(hudX, y, "[Objetivo]:", WHITE_BRIGHT);
        PrintLine(hudX, y, "Encuentra la puerta");
        PrintLine(hudX, y, "Destruye % con bombas");
        PrintLine(hudX, y, "Evita morir con fuego");
        y++;

        // Controles
        PrintLine(hudX, y, "Controles:", WHITE_BRIGHT);
        PrintLine(hudX, y, "[WASD]: Moverse");
        PrintLine(hudX, y, "[B]: Bomba");

        std::cout << RESET;
    }

private:
    void PrintLine(int x, int &y, const std::string &text, const std::string &color = GRAY_BRIGHT) const
    {
        std::cout << "\033[" << y << ";" << x << "H" << color << text << RESET;
        y++;
    }
};

#endif // HUD_BOMBERMAN_H
