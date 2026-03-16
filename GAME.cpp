#include "DxLib.h"
#include "Game.h"
#include "EnemyManager.h"
#include "BulletManager.h"

#include <algorithm>
#include <vector>
#include <functional>

Game* gGame = nullptr;

// 画像を中央に描画する関数
void DrawCenter(int img)
{
    int w, h;
    GetGraphSize(img, &w, &h);
    DrawGraph((1280 - w) / 2, (720 - h) / 2, img, TRUE);
}

Game::Game() : player(640, 360)
{
    imgTitle = LoadGraph("assets/title.png");
    imgGameOver = LoadGraph("assets/gameover.png");
    imgClear = LoadGraph("assets/clear.png");
}

void Game::Init()
{
    wave = 1;

    spawnTimer = 0;
    waveTextTimer = 120;
    hitStopTimer = 0;

    gameClear = false;

    // プレイヤーリセット
    player = Player(640, 360);

    EnemyManager::enemies.clear();
    EnemyManager::Init(5 * wave, wave);

    SetMouseDispFlag(FALSE);

    state = STATE_WAVE_START;
}

void Game::Update()
{
    static int spacePrev = 0;
    int spaceNow = CheckHitKey(KEY_INPUT_SPACE);

    switch (state)
    {

    case STATE_TITLE:

        if (spaceNow && !spacePrev)
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

        if (spaceNow && !spacePrev)
        {
            state = STATE_TITLE;
        }

        break;


    case STATE_GAMEOVER:

        if (spaceNow && !spacePrev)
        {
            state = STATE_TITLE;
        }

        break;
    }

    spacePrev = spaceNow;
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

    // プレイヤー死亡
    if (player.isDead)
    {
        state = STATE_GAMEOVER;
        return;
    }

    // 敵が全滅
    if (EnemyManager::enemies.empty())
    {
        spawnTimer++;

        if (spawnTimer > 120)
        {
            spawnTimer = 0;

            if (wave == 3)
            {
                gameClear = true;
                state = STATE_CLEAR;
            }
            else
            {
                wave++;

                waveTextTimer = 120;

                EnemyManager::Init(5 * wave, wave);

                state = STATE_WAVE_START;
            }
        }
    }
    else
    {
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

        DrawCenter(imgTitle);
        DrawString(560, 520, "PRESS SPACE", GetColor(255, 255, 255));

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

        DrawCenter(imgClear);
        DrawString(560, 520, "PRESS SPACE", GetColor(255, 255, 255));

        break;


    case STATE_GAMEOVER:

        DrawGame();

        DrawCenter(imgGameOver);
        DrawString(560, 520, "PRESS SPACE", GetColor(255, 255, 255));

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