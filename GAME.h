#pragma once
#include "Player.h"
#include "Map.h"

class Game
{
public:
    Player player;
    Map map;

    int hitStopTimer = 0;

    int spawnTimer = 0;
    int wave = 1;
    int waveTextTimer = 0;

    bool gameClear = false;

    Game();

    void Init();
    void Update();
    void Draw();
};

// ƒNƒ‰ƒX‚ÌŠO‚É‘‚­
extern Game* gGame;