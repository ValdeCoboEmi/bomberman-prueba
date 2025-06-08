#include "Game.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <conio.h> // Para _kbhit() y _getch() en Windows
#include <limits> // para std::numeric_limits

#ifdef _WIN32
#include <windows.h>
#endif

Game::Game() : currentLevel(1), isRunning(true)
{
#ifdef _WIN32
    HWND console = GetConsoleWindow();
    RECT r;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, 1000, 720, TRUE);
    // ⛔ Ocultar el cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);

#endif

    loadLevel(currentLevel);
}

void Game::run()
{
    while (isRunning)
    {
        Utils::clearScreen();
        hud.draw(player, currentLevel, map.getWidth());
        map.draw(player.getX(), player.getY(), bombs, 1, 1);

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

        // Control de FPS
        Utils::sleep(30);
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

    if (tile != '#' && tile != '0' && tile != '~' && tile != 'H' && tile != 'A') // No es pared ni bomba
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
    // Construir el nombre del archivo según el nivel actual
    std::string filename = "maps-bomberman/easy-levels/level" + std::to_string(level) + ".txt";
    std::ifstream file(filename);

    if (!file)
    {
        Utils::clearScreen();
        std::cout << "\n🎉 ¡Felicidades! Has completado todas las salas.\n";
        std::cout << "\nPresiona Enter para salir...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // Espera que el jugador presione Enter
        std::cin.get(); 
        isRunning = false;
        return;
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line))
    {
        lines.push_back(line);
    }

    if (lines.empty())
    {
        std::cerr << "El archivo de nivel está vacío: " << filename << "\n";
        isRunning = false;
        return;
    }

    // Cargar el mapa y reiniciar estado del juego
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

        if (tile != '#' && tile != '~' && tile != ']' && tile != '/' && tile != '0')
        {
            if (tile == '%')
                map.setTile(nx, ny, (rand() % 4 == 0) ? 'B' : '*');
            else
                map.setTile(nx, ny, '*');
        }
    };

    applyExplosion(0, 0);  // centro
    applyExplosion(0, -1); // arriba
    applyExplosion(0, 1);  // abajo
    applyExplosion(-1, 0); // izquierda
    applyExplosion(1, 0);  // derecha

#ifdef _WIN32
    Beep(800, 50);
    Beep(600, 50);
    Beep(300, 100);

#endif

    // 🔧 NO limpiar pantalla aquí
    // Usar mismos offsets que en el bucle principal
    Utils::clearScreen();
    hud.draw(player, currentLevel, map.getWidth());
    map.draw(player.getX(), player.getY(), bombs, 1, 1);

    Utils::sleep(200); // Mostrar '*'

    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
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
