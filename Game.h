#ifndef GAME_H
#define GAME_H

// Archivos propios del juego
#include "Map.h"
#include "Player.h"
#include "Bomb.h"
#include "HUD.h"
#include "Utils.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <conio.h>   // Para _kbhit() y _getch() en Windows
#include <limits>    // Para std::numeric_limits

#ifdef _WIN32
#include <windows.h> // Para manipular la ventana de la consola en Windows
#endif

// Clase principal del juego
class Game {
public:
    Game();      // Constructor: inicializa el juego
    void run();  // Bucle principal del juego

private:
    Map map;                  // Mapa actual del nivel
    Player player;            // Jugador
    HUD hud;                  // Interfaz gráfica (vidas, bombas, etc.)
    std::vector<Bomb> bombs;  // Bombas activas en el mapa
    int currentLevel;         // Nivel actual
    bool isRunning;           // Estado del juego

    void processInput(char input);      // Manejo de entradas del jugador
    void loadLevel(int level);          // Cargar un nuevo nivel
    void explode(int x, int y);         // Ejecutar explosión en coordenadas
};

// IMPLEMENTACIÓN

// Constructor del juego
Game::Game() : currentLevel(1), isRunning(true)
{
#ifdef _WIN32
    // Ajuste de tamaño de la ventana y ocultar cursor en consola (solo en Windows)
    HWND console = GetConsoleWindow();
    RECT r;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, 1000, 720, TRUE);

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;  // Oculta el cursor
    SetConsoleCursorInfo(hOut, &cursorInfo);
#endif

    loadLevel(currentLevel);  // Cargar el primer nivel
}

// Bucle principal del juego
void Game::run()
{
    while (isRunning)
    {
        Utils::clearScreen();  // Limpia la consola

        // Dibuja el mapa, el jugador y las bombas
        map.draw(player.getX(), player.getY(), bombs, 1, 1);
        hud.draw(player, currentLevel, map.getWidth());

        // Actualiza todas las bombas (por ahora, `update()` no hace nada)
        for (auto &bomb : bombs)
            bomb.update();

        // Revisar si alguna bomba ha explotado
        for (size_t i = 0; i < bombs.size();)
        {
            if (bombs[i].hasExploded())
            {
                explode(bombs[i].getX(), bombs[i].getY());  // Ejecuta la explosión
                bombs.erase(bombs.begin() + i);             // Elimina la bomba
            }
            else
            {
                ++i;
            }
        }

        // Leer entrada del usuario si se presiona alguna tecla
        if (_kbhit())
        {
            char input = _getch();
            processInput(input);
        }

        // Verificar si el jugador ha perdido
        if (player.getLives() <= 0)
        {
            Utils::clearScreen();
            std::cout << "\nHas perdido todas tus vidas. ¡Game Over!\n";
            isRunning = false;
        }

        // Control de la velocidad del juego (30 ms por frame)
        Utils::sleep(30);
    }
}

// Manejo de la entrada del usuario
void Game::processInput(char input)
{
    int dx = 0, dy = 0;

    switch (input)
    {
    case 'w': dy = -1; break;     // Mover arriba
    case 's': dy = 1; break;      // Mover abajo
    case 'a': dx = -1; break;     // Mover izquierda
    case 'd': dx = 1; break;      // Mover derecha
    case 'b': // Colocar bomba
        if (player.canPlaceBomb())
        {
            bombs.push_back(Bomb(player.getX(), player.getY())); // Añadir nueva bomba
            map.setTile(player.getX(), player.getY(), '0');       // Marcar con bomba
            player.placeBomb();                                   // Registrar uso
        }
        return;
    }

    int newX = player.getX() + dx;
    int newY = player.getY() + dy;
    char tile = map.getTile(newX, newY);  // Qué hay en la celda destino

    // Validar si se puede mover a esa celda
    if (tile != '#' && tile != '0' && tile != '~' && tile != 'H' && tile != 'A')
    {
        if (tile == '/')
        {
            currentLevel++;          // Ir al siguiente nivel
            loadLevel(currentLevel);
            return;
        }

        if (tile == 'B')             // Potenciador de bombas
        {
            player.addMaxBomb();    // Aumentar capacidad
            map.setTile(newX, newY, ' ');
        }

        // Mover jugador y limpiar celda anterior
        map.setTile(player.getX(), player.getY(), ' ');
        player.move(dx, dy, tile);
    }
}

// Cargar un nivel desde archivo
void Game::loadLevel(int level)
{
    std::string filename = "maps-bomberman/easy-levels/level" + std::to_string(level) + ".txt";
    std::ifstream file(filename);

    if (!file)
    {
        // Si no hay más niveles, felicitar al jugador y cerrar juego
        Utils::clearScreen();
        std::cout << "\n🎉 ¡Felicidades! Has completado todas las salas.\n";
        std::cout << "\nPresiona Enter para salir...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        isRunning = false;
        return;
    }

    // Leer el contenido del archivo
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

    // Cargar el mapa y reiniciar bombas y posición del jugador
    map.loadFromFile(filename);
    bombs.clear();
    player.setPosition(map.getSpawnX(), map.getSpawnY());
}

// Ejecutar explosión en la posición (x, y)
void Game::explode(int x, int y)
{
    // Función lambda para aplicar daño/explosión en una dirección
    auto applyExplosion = [&](int dx, int dy)
    {
        int nx = x + dx;
        int ny = y + dy;
        char tile = map.getTile(nx, ny);

        // Si el jugador está en el área de explosión, pierde vida
        if (nx == player.getX() && ny == player.getY())
        {
            player.loseLife();
        }

        // No destruir muros sólidos ni agua, ni otros elementos importantes
        if (tile != '#' && tile != '~' && tile != ']' && tile != '/' && tile != '0')
        {
            if (tile == '%')  // Escombro: puede dar bonificación
                map.setTile(nx, ny, (rand() % 4 == 0) ? 'B' : '*');
            else              // Otro bloque: mostrar explosión
                map.setTile(nx, ny, '*');
        }
    };

    // Aplicar explosión en cruz (centro + 4 direcciones)
    applyExplosion(0, 0);
    applyExplosion(0, -1);
    applyExplosion(0, 1);
    applyExplosion(-1, 0);
    applyExplosion(1, 0);

#ifdef _WIN32
    // Efectos de sonido (solo en Windows)
    Beep(800, 50);
    Beep(600, 50);
    Beep(300, 100);
#endif

    // Mostrar la explosión brevemente
    Utils::clearScreen();
    hud.draw(player, currentLevel, map.getWidth());
    map.draw(player.getX(), player.getY(), bombs, 1, 1);
    Utils::sleep(200);

    // Limpiar la explosión y restaurar el terreno
    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            // Solo en cruz, no diagonales
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

#endif // GAME_H
