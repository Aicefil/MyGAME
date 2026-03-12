#pragma once
#include "Player.h"
#include "Map.h"

class Game
{
private:

    enum GameState
    {
        STATE_TITLE,
        STATE_WAVE_START,
        STATE_PLAYING,
        STATE_CLEAR,
        STATE_GAMEOVER
    };

    GameState state = STATE_TITLE;

    int spawnTimer = 0;
    int waveTextTimer = 0;

public:

    Player player;
    Map map;

    int hitStopTimer = 0;

    int wave = 1;
    bool gameClear = false;

    Game();

    void Init();

    void Update();
    void Draw();

    void UpdatePlaying();
    void DrawGame();
};

extern Game* gGame;