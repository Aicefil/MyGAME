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
    waveTextTimer = 120;

    EnemyManager::Init(2 + wave * 2, wave);
}

void Game::Update()
{
    if (waveTextTimer > 0)
        waveTextTimer--;

    // ヒットストップ中
    if (hitStopTimer > 0)
    {
        hitStopTimer--;
        return;
    }

    player.Update(map);
    BulletManager::UpdateAll(map);
    EnemyManager::UpdateAll(player.x, player.y, map);

    // 敵が全滅したら
    if (EnemyManager::enemies.empty())
    {
        spawnTimer++;

        if (spawnTimer > 120)
        {
            spawnTimer = 0;

            // 3ウェーブまで
            if (wave < 3)
            {
                wave++;
                waveTextTimer = 120;

                EnemyManager::Init(2 + wave * 2, wave);
            }
            else
            {
                // ゲームクリア
                gameClear = true;
                return;
            }
        }
    }
}

void Game::Draw()
{
    // 背景・障害物
    map.DrawBackground();
    map.DrawObstacles();

    struct DrawObj
    {
        float y;
        std::function<void()> drawFunc;
    };

    std::vector<DrawObj> drawList;

    // WAVE表示
    if (waveTextTimer > 0)
    {
        DrawFormatString(
            540,
            200,
            GetColor(255, 255, 0),
            "WAVE %d",
            wave
        );
    }

    // ゲームクリア 
    if (gameClear)
    {
        DrawString(
            560,
            350,
            "GAME CLEAR!",
            GetColor(255, 255, 0)
        );
    }

    // プレイヤー
    drawList.push_back({ player.y, [&]() { player.Draw(); } });

    // 敵
    for (auto& e : EnemyManager::enemies)
        drawList.push_back({ e.y, [&e]() { e.Draw(); } });

    // 弾
    for (auto& b : BulletManager::bullets)
        drawList.push_back({ b.y, [&b]() { b.Draw(); } });

    // Yソート
    std::sort(drawList.begin(), drawList.end(),
        [](const DrawObj& a, const DrawObj& b) { return a.y < b.y; });

    // 描画
    for (auto& obj : drawList)
        obj.drawFunc();

    // カーソル
    int mx, my;
    GetMousePoint(&mx, &my);
    DrawCircle(mx, my, 5, GetColor(255, 255, 0), TRUE);
}