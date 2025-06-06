#include "Map.h"
#include <fstream>
#include <iostream>
#include "Utils.h"
#include "Colors.h" // Asegúrate de tener los defines de color aquí

Map::Map() {}

bool Map::loadFromFile(const std::string &filename)
{
    grid.clear();
    std::ifstream file(filename);
    if (!file.is_open())
        return false;

    std::string line;
    int y = 0;
    while (std::getline(file, line))
    {
        grid.push_back(line);
        // Buscar la puerta
        size_t pos = line.find(']');
        if (pos != std::string::npos)
        {
            spawnX = pos + 1; // El personaje aparece a la derecha de la puerta
            spawnY = y;
        }
        ++y;
    }
    return true;
}

void Map::draw(int playerX, int playerY, const std::vector<Bomb> &bombs, int offsetX, int offsetY)
{
    for (int y = 0; y < getHeight(); ++y)
    {
        for (int x = 0; x < getWidth(); ++x)
        {
            Utils::moveCursor(offsetX + x, y + offsetY);

            if (x == playerX && y == playerY)
            {
                std::cout << PINK << "o" << RESET;
            }
            else
            {
                bool isBomb = false;

                for (const auto &bomb : bombs)
                {
                    if (bomb.getX() == x && bomb.getY() == y)
                    {
                        isBomb = true;
                        break;
                    }
                }

                if (isBomb)
                {
                    std::cout << ORANGE << "0" << RESET;
                }
                else
                {
                    char tile = grid[y][x];
                    switch (tile)
                    {
                    case '#':
                        std::cout << GRAY << "#" << RESET;
                        break;
                    case ']':
                        std::cout << GRAY << "]" << RESET;
                        break;
                    case 'B':
                        std::cout << RED << "B" << RESET;
                        break;
                    case 'A':
                        std::cout << GREEN << "#" << RESET;
                        break; // árbol
                    case 'H':
                        std::cout << GREEN << "O" << RESET;
                        break; // árbol
                    case 'T':
                        std::cout << BROWN << "W" << RESET;
                        break; // tronco y hojas como árbol
                    case '~':
                        std::cout << BLUE << "~" << RESET;
                        break; // lago
                    case '*':
                        std::cout << YELLOW_BRIGHT << "*" << RESET;
                        break; // explosión
                    default:
                        std::cout << tile;
                        break;
                    }
                }
            }
        }
    }
}


char Map::getTile(int x, int y) const
{
    if (y >= 0 && y < grid.size() && x >= 0 && x < grid[y].size())
    {
        return grid[y][x];
    }
    return ' ';
}

void Map::setTile(int x, int y, char value)
{
    if (y >= 0 && y < grid.size() && x >= 0 && x < grid[y].size())
    {
        grid[y][x] = value;
    }
}

int Map::getWidth() const {
    if (grid.empty()) return 0;
    return grid[0].size();
}

int Map::getHeight() const
{
    return grid.size();
}
