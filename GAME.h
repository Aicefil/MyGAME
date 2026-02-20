#pragma once
#include "Player.h"
#include "Map.h"
#include "Camera.h"

class Game
{
public:
    Player player;
    Map map;
    Camera camera;

    int hitStopTimer = 0;

    int spawnTimer = 0;
    int wave = 1;

    Game();

    void Init();
    void Update();
    void Draw();
};

// ƒNƒ‰ƒX‚ÌŠO‚É‘‚­
extern Game* gGame;