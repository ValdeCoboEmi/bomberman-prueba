#ifndef GAME_H
#define GAME_H

#include "Utils.h"
#include "Map.h"
#include "BombRenderer.h"
#include "Player.h"
#include "HUDBomberman.h"
#include "Bomb.h"

#include <fstream>
#include <iostream>
#include <conio.h>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif

const int MAX_BOMBS = 25;

class Game
{
public:
    Game();
    void run();

private:
    Map map;
    Player player;
    HUDBomberman hud;
    Bomb bombs[MAX_BOMBS];
    int bombCount = 0;

    int currentLevel;
    bool isRunning;
    int offsetX = 1; 
    int offsetY = 1;

    void processInput(char input);
    void loadLevel(int level);
    void handleExplosion(int i);
};

Game::Game() : currentLevel(1), isRunning(true), bombCount(0)
{
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

void Game::run()
{
    player.ActivateControlB(true);
    while (isRunning)
    {
        Utils::clearScreen();
        map.draw(player.GetX(), player.GetY(), offsetX, offsetY);
        BombRenderer::draw(bombs, bombCount, 1, 1);
        hud.Draw(player, currentLevel, map.getWidth());

        for (int i = 0; i < bombCount;)
        {
            if (bombs[i].HasExploded())
            {
                handleExplosion(i);
                for (int j = i; j < bombCount - 1; ++j)
                    bombs[j] = bombs[j + 1];
                --bombCount;
            }
            else
            {
                ++i;
            }
        }

        if (_kbhit())
            processInput(_getch());

        if (player.GetLives() <= 0)
        {
            Utils::clearScreen();
            std::cout << "\nHas perdido todas tus vidas. ¡Game Over!\n";
            isRunning = false;
        }

        Utils::sleep(30);
    }
}

void Game::processInput(char input)
{
    if (input == 'b' && player.IsControlBActive())
    {
        if (player.CanPlaceBomb() && (bombCount < MAX_BOMBS))
        {
            bombs[bombCount++] = Bomb(player.GetX(), player.GetY());
            map.setTile(player.GetX(), player.GetY(), '0');
            player.PlaceBomb();
        }
        return;
    }

    bool changeLevel = player.TryMove(input, map);
    if (changeLevel)
    {
        currentLevel++;
        loadLevel(currentLevel);
    }
}

void Game::loadLevel(int level)
{
    std::string filename = "maps-bomberman/easy-levels/level" + std::to_string(level) + ".txt";
    std::ifstream file(filename);

    if (!file)
    {
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
    player.SetPosition(map.getSpawnX(), map.getSpawnY());
}

void Game::handleExplosion(int i)
{
    int x = bombs[i].GetX();
    int y = bombs[i].GetY();

    auto applyExplosion = [&](int dx, int dy)
    {
        int nx = x + dx, ny = y + dy;
        char tile = map.getTile(nx, ny);

        if (nx == player.GetX() && ny == player.GetY())
            player.LoseLife();

        if (tile != '#' && tile != '~' && tile != ']' && tile != '/' && tile != '0')
        {
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
    hud.Draw(player, currentLevel, map.getWidth());
    map.draw(player.GetX(), player.GetY(), offsetX, offsetY);
    BombRenderer::draw(bombs, bombCount, 1, 1);
    Utils::sleep(30);

    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            if ((dx == 0 || dy == 0) && !(dx != 0 && dy != 0))
            {
                int nx = x + dx, ny = y + dy;
                char tile = map.getTile(nx, ny);
                if (tile == '*' || tile == '0')
                    map.setTile(nx, ny, ' ');
            }
        }
    }
}

#endif // GAME_H
