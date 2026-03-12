#include "DxLib.h"
#include "Game.h"
#include "EnemyManager.h"
#include "BulletManager.h"

#include <algorithm>
#include <vector>
#include <functional>

Game* gGame = nullptr;

Game::Game() : player(640, 360)
{
}

void Game::Init()
{
    wave = 1;
    spawnTimer = 0;
    waveTextTimer = 120;

    gameClear = false;

    EnemyManager::Init(2 + wave * 2, wave);

    SetMouseDispFlag(FALSE);  // OSカーソル消す

    state = STATE_WAVE_START;
}

void Game::Update()
{
    switch (state)
    {

    case STATE_TITLE:

        if (GetMouseInput() & MOUSE_INPUT_LEFT)
        {
            Init();
        }

        break;


    case STATE_WAVE_START:

        waveTextTimer--;

        if (waveTextTimer <= 0)
        {
            state = STATE_PLAYING;
        }

        break;


    case STATE_PLAYING:

        UpdatePlaying();

        break;


    case STATE_CLEAR:

        if (GetMouseInput() & MOUSE_INPUT_LEFT)
        {
            state = STATE_TITLE;
        }

        break;


    case STATE_GAMEOVER:

        if (GetMouseInput() & MOUSE_INPUT_LEFT)
        {
            state = STATE_TITLE;
        }

        break;
    }
}

void Game::UpdatePlaying()
{
    if (hitStopTimer > 0)
    {
        hitStopTimer--;
        return;
    }

    player.Update(map);

    BulletManager::UpdateAll(map);

    EnemyManager::UpdateAll(player.x, player.y, map);

    // ===== プレイヤー死亡 =====
    if (player.isDead)
    {
        state = STATE_GAMEOVER;
        return;
    }

    // ===== 敵が全滅したら =====
    if (EnemyManager::enemies.empty())
    {
        spawnTimer++;

        if (spawnTimer > 120)
        {
            spawnTimer = 0;

            // 次Wave
            if (wave < 3)
            {
                wave++;

                waveTextTimer = 120;

                EnemyManager::Init(2 + wave * 2, wave);

                state = STATE_WAVE_START;
            }
            else
            {
                // 全Wave終了
                gameClear = true;
                state = STATE_CLEAR;
            }
        }
    }
    else
    {
        // 敵がいるならタイマーリセット
        spawnTimer = 0;
    }
}

void Game::Draw()
{

    map.DrawBackground();
    map.DrawObstacles();

    switch (state)
    {

    case STATE_TITLE:

        DrawString(560, 300, "CLICK TO START", GetColor(255, 255, 255));

        break;


    case STATE_WAVE_START:

        DrawGame();

        DrawFormatString(
            540,
            200,
            GetColor(255, 255, 0),
            "WAVE %d",
            wave
        );

        break;


    case STATE_PLAYING:

        DrawGame();

        break;


    case STATE_CLEAR:

        DrawGame();

        DrawString(
            560,
            350,
            "GAME CLEAR!",
            GetColor(255, 255, 0)
        );

        break;


    case STATE_GAMEOVER:

        DrawGame();

        DrawString(
            560,
            350,
            "GAME OVER",
            GetColor(255, 0, 0)
        );

        break;
    }

    int mx, my;
    GetMousePoint(&mx, &my);

    int color = GetColor(255, 220, 120);

    // 中心点
    DrawCircle(mx, my, 3, color, TRUE);

    // 外側リング
    DrawCircle(mx, my, 10, color, FALSE);

    // 十字線
    DrawLine(mx - 16, my, mx - 6, my, color);
    DrawLine(mx + 6, my, mx + 16, my, color);
    DrawLine(mx, my - 16, mx, my - 6, color);
    DrawLine(mx, my + 6, mx, my + 16, color);
}

void Game::DrawGame()
{

    struct DrawObj
    {
        float y;
        std::function<void()> drawFunc;
    };

    std::vector<DrawObj> drawList;


    drawList.push_back({ player.y, [&]() { player.Draw(); } });


    for (auto& e : EnemyManager::enemies)
        drawList.push_back({ e.y, [&e]() { e.Draw(); } });


    for (auto& b : BulletManager::bullets)
        drawList.push_back({ b.y, [&b]() { b.Draw(); } });


    std::sort(
        drawList.begin(),
        drawList.end(),
        [](const DrawObj& a, const DrawObj& b)
        {
            return a.y < b.y;
        }
    );


    for (auto& obj : drawList)
        obj.drawFunc();
}