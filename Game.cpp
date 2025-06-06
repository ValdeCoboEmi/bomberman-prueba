#include "Game.h"
#include "Utils.h"
#include <iostream>
#include <conio.h> // Para _kbhit() y _getch() en Windows

#ifdef _WIN32
#include <windows.h>
#endif

Game::Game() : currentLevel(1), isRunning(true)
{
#ifdef _WIN32
    // Tamaño fijo para ventana de consola en Windows
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, 1200, 720, TRUE);

#endif

    loadLevel(currentLevel);
}

void Game::run()
{
    while (isRunning)
    {
        Utils::clearScreen();
        hud.draw(player, currentLevel, map.getWidth());
        map.draw(player.getX(), player.getY(), bombs, hud.getHeight());


        for (auto &bomb : bombs)
            bomb.update();

        // Revisar explosiones
        for (size_t i = 0; i < bombs.size();)
        {
            if (bombs[i].hasExploded())
            {
                explode(bombs[i].getX(), bombs[i].getY());
                bombs.erase(bombs.begin() + i);
            }
            else
            {
                ++i;
            }
        }

        if (_kbhit())
        {
            char input = _getch();
            processInput(input);
        }

        if (player.getLives() <= 0)
        {
            Utils::clearScreen();
            std::cout << "\nHas perdido todas tus vidas. ¡Game Over!\n";
            isRunning = false;
        }

        Utils::sleep(100); // Control de FPS
    }
}

void Game::processInput(char input)
{
    int dx = 0, dy = 0;
    switch (input)
    {
    case 'w':
        dy = -1;
        break;
    case 's':
        dy = 1;
        break;
    case 'a':
        dx = -1;
        break;
    case 'd':
        dx = 1;
        break;
    case 'b': // Colocar bomba
        if (player.canPlaceBomb())
        {
            bombs.push_back(Bomb(player.getX(), player.getY()));
            map.setTile(player.getX(), player.getY(), '0');
            player.placeBomb();
        }

        return;
    }

    int newX = player.getX() + dx;
    int newY = player.getY() + dy;
    char tile = map.getTile(newX, newY);

    if (tile != '#' && tile != '0' && tile != 'B' && tile != '~' && tile != 'H' && tile != 'A') // No es pared ni bomba
    {
        if (tile == '/')
        {
            currentLevel++;
            loadLevel(currentLevel);
            return;
        }
        if (tile == 'B')
        {
            player.addMaxBomb();
            map.setTile(newX, newY, ' ');
        }

        map.setTile(player.getX(), player.getY(), ' ');
        player.move(dx, dy, tile);
    }
}

void Game::loadLevel(int level)
{
    std::string filename = "maps-bomberman/easy.txt";
    map.loadFromFile(filename);

    bombs.clear();
    player.setPosition(map.getSpawnX(), map.getSpawnY());
}
void Game::explode(int x, int y)
{
    auto applyExplosion = [&](int dx, int dy)
    {
        int nx = x + dx;
        int ny = y + dy;
        char tile = map.getTile(nx, ny);

        if (nx == player.getX() && ny == player.getY())
        {
            player.loseLife();
        }

        // Solo explota si no es pared
        if (tile != '#' && tile != '~' && tile != ']' && tile != '/')
        {
            if (tile == '%')
            {
                // 25% de probabilidad de aparecer B
                map.setTile(nx, ny, (rand() % 4 == 0) ? 'B' : '*');
            }
            else
            {
                map.setTile(nx, ny, '*'); // mostrar explosión
            }
        }
    };

    applyExplosion(0, 0);  // centro
    applyExplosion(0, -1); // arriba
    applyExplosion(0, 1);  // abajo
    applyExplosion(-1, 0); // izquierda
    applyExplosion(1, 0);  // derecha

    // Redibuja para que el mapa muestre la explosión
    Utils::clearScreen();
    hud.draw(player, currentLevel);
    map.draw(player.getX(), player.getY(), bombs, hud.getHeight());

    Utils::sleep(200); // Mostrar explosión durante 200ms

    // Limpia explosión (solo borra '*')
    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            // Solo los puntos de cruz (centro y ejes)
            if ((dx == 0 || dy == 0) && !(dx != 0 && dy != 0))
            {
                int nx = x + dx;
                int ny = y + dy;

                if (map.getTile(nx, ny) == '*')
                    map.setTile(nx, ny, ' ');
            }
        }
    }
}
