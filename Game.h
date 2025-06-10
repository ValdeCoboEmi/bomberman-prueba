#ifndef GAME_H
#define GAME_H

// Archivos propios del juego
#include "Map.h"
#include "Player.h"
#include "Bomb.h"
#include "HUD.h"
#include "Utils.h"

#include <fstream>
#include <iostream>
#include <conio.h>   // Para _kbhit() y _getch() en Windows
#include <limits>    // Para std::numeric_limits

#ifdef _WIN32
#include <windows.h> // Para manipular la ventana de la consola en Windows
#endif

class Game {
public:
    Game();      // Constructor: inicializa el juego
    void run();  // Bucle principal del juego

private:
    Map map;
    Player player;
    HUD hud;
    Bomb bombs[MAX_BOMBS];  // Arreglo estático de bombas
    int bombCount = 0;

    int currentLevel;
    bool isRunning;

    void processInput(char input);
    void loadLevel(int level);
    void explode(int x, int y);
};

// IMPLEMENTACIÓN

Game::Game() : currentLevel(1), isRunning(true), bombCount(0) {
#ifdef _WIN32
    HWND console = GetConsoleWindow();
    RECT r;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, 1000, 720, TRUE);

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);
#endif
    loadLevel(currentLevel);
}

void Game::run() {
    while (isRunning) {
        Utils::clearScreen();

        map.draw(player.getX(), player.getY(), bombs, bombCount, 1, 1);
        hud.draw(player, currentLevel, map.getWidth());

        // Actualizar bombas
        for (int i = 0; i < bombCount; ++i)
            bombs[i].update();

        // Manejar explosiones
        for (int i = 0; i < bombCount;) {
            if (bombs[i].hasExploded()) {
                explode(bombs[i].getX(), bombs[i].getY());

                // Eliminar bomba del arreglo
                for (int j = i; j < bombCount - 1; ++j)
                    bombs[j] = bombs[j + 1];
                --bombCount;
            } else {
                ++i;
            }
        }

        if (_kbhit()) {
            char input = _getch();
            processInput(input);
        }

        if (player.getLives() <= 0) {
            Utils::clearScreen();
            std::cout << "\nHas perdido todas tus vidas. ¡Game Over!\n";
            isRunning = false;
        }

        Utils::sleep(30);
    }
}

void Game::processInput(char input) {
    int dx = 0, dy = 0;

    switch (input) {
        case 'w': dy = -1; break;
        case 's': dy = 1; break;
        case 'a': dx = -1; break;
        case 'd': dx = 1; break;
        case 'b':
            if (player.canPlaceBomb() && bombCount < MAX_BOMBS) {
                bombs[bombCount++] = Bomb(player.getX(), player.getY());
                map.setTile(player.getX(), player.getY(), '0');
                player.placeBomb();
            }
            return;
    }

    int newX = player.getX() + dx;
    int newY = player.getY() + dy;
    char tile = map.getTile(newX, newY);

    if (tile != '#' && tile != '0' && tile != '~' && tile != 'H' && tile != 'A') {
        if (tile == '/') {
            currentLevel++;
            loadLevel(currentLevel);
            return;
        }
        if (tile == 'B') {
            player.addMaxBomb();
            map.setTile(newX, newY, ' ');
        }

        map.setTile(player.getX(), player.getY(), ' ');
        player.move(dx, dy, tile);
    }
}

void Game::loadLevel(int level) {
    std::string filename = "maps-bomberman/easy-levels/level" + std::to_string(level) + ".txt";
    std::ifstream file(filename);

    if (!file) {
        Utils::clearScreen();
        std::cout << "\n🎉 ¡Felicidades! Has completado todas las salas.\n";
        std::cout << "\nPresiona Enter para salir...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        isRunning = false;
        return;
    }

    map.loadFromFile(filename);
    bombCount = 0;
    player.setPosition(map.getSpawnX(), map.getSpawnY());
}

void Game::explode(int x, int y) {
    auto applyExplosion = [&](int dx, int dy) {
        int nx = x + dx;
        int ny = y + dy;
        char tile = map.getTile(nx, ny);

        if (nx == player.getX() && ny == player.getY()) {
            player.loseLife();
        }

        if (tile != '#' && tile != '~' && tile != ']' && tile != '/' && tile != '0') {
            if (tile == '%')
                map.setTile(nx, ny, (rand() % 4 == 0) ? 'B' : '*');
            else
                map.setTile(nx, ny, '*');
        }
    };

    applyExplosion(0, 0);
    applyExplosion(0, -1);
    applyExplosion(0, 1);
    applyExplosion(-1, 0);
    applyExplosion(1, 0);

#ifdef _WIN32
    Beep(800, 50);
    Beep(600, 50);
    Beep(300, 100);
#endif

    Utils::clearScreen();
    hud.draw(player, currentLevel, map.getWidth());
    map.draw(player.getX(), player.getY(), bombs, bombCount, 1, 1);
    Utils::sleep(200);

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if ((dx == 0 || dy == 0) && !(dx != 0 && dy != 0)) {
                int nx = x + dx;
                int ny = y + dy;
                char tile = map.getTile(nx, ny);
                if (tile == '*')
                    map.setTile(nx, ny, ' ');
            }
        }
    }
}

#endif // GAME_H
