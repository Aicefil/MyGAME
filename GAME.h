#pragma once
#include "Player.h"
#include "Map.h"

#include <string>
#include <vector>

class Game
{
private:

    enum GameState
    {
        STATE_TITLE,
        STATE_WAVE_START,
        STATE_PLAYING,
        STATE_CLEAR,
        STATE_GAMEOVER,
        STATE_RESULT   
    };

    GameState state = STATE_TITLE;

    int resultWaitTimer = 0;  // クリア／ゲームオーバー表示後の待機時間（フレーム）

    int spawnTimer = 0;
    int waveTextTimer = 0;

    int waveTimer = 0;
    int waveTimeLimit = 600;

    // ===== リザルト画面用 =====
    std::string resultText;           // 「GAME OVER」「GAME CLEAR」
    int resultAlpha = 0;              // 背景フェード用
    int resultScoreDisplay = 0;       // スコアカウントアップ用
    int blinkTimer = 0;               // PRESS SPACE点滅用

    struct Particle                    // キラキラエフェクト用
    {
        float x, y;
        int r;
        int alpha;
    };
    std::vector<Particle> resultParticles;

public:

    int score = 0;
    void AddScore(int value) { score += value; }
    int GetScore() const { return score; }

    int imgTitle;
    int imgGameOver;
    int imgClear;

    Player player;
    Map map;

    int mousePrev = 0;
    int hitStopTimer = 0;
    int wave = 1;
    bool gameClear = false;

    Game();

    void Init();

    void Update();
    void Draw();

    void UpdatePlaying();
    void DrawGame();

    // ===== リザルト用関数 =====
    void ShowResult(const std::string& msg);
    void UpdateResult();
};

extern Game* gGame;