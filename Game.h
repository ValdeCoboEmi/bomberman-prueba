#ifndef GAME_H
#define GAME_H

#include "Map.h"
#include "Player.h"
#include "Bomb.h"
#include "HUD.h"
#include <vector>

class Game {
public:
    Game();
    void run();

private:
    Map map;
    Player player;
    HUD hud;
    std::vector<Bomb> bombs;
    int currentLevel;
    bool isRunning;

    void processInput(char input);
    void loadLevel(int level);
    void explode(int x, int y); 
};

#endif
