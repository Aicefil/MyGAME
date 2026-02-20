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
    EnemyManager::Init(5);
}

void Game::Update()
{
    // ヒットストップ中
    if (hitStopTimer > 0)
    {
        hitStopTimer--;
        return;  // 更新停止
    }

    player.Update(map);
    BulletManager::UpdateAll(map);
    EnemyManager::UpdateAll(player.x, player.y);

    if (EnemyManager::enemies.empty())
    {
        spawnTimer++;

        if (spawnTimer > 120)
        {
            spawnTimer = 0;
            wave++;
            EnemyManager::Init(wave + 3);
        }
    }
}

void Game::Draw()
{
    map.Draw();

    // 描画用構造体
    struct DrawObj
    {
        float y;
        std::function<void()> drawFunc;
    };

    std::vector<DrawObj> drawList;

    // プレイヤー
    drawList.push_back({
        player.y,
        [&]() { player.Draw(); }
        });

    // 敵
    for (auto& e : EnemyManager::enemies)
    {
        drawList.push_back({
            e.y,
            [&e]() { e.Draw(); }
            });
    }

    // 弾
    for (auto& b : BulletManager::bullets)
    {
        drawList.push_back({
            b.y,
            [&b]() { b.Draw(); }
            });
    }

    // Yソート
    std::sort(drawList.begin(), drawList.end(),
        [](const DrawObj& a, const DrawObj& b)
        {
            return a.y < b.y;
        });

    // 描画実行
    for (auto& obj : drawList)
        obj.drawFunc();

    //独自カーソル
    int mx, my;
    GetMousePoint(&mx, &my);

    DrawCircle(mx, my, 5,
        GetColor(255, 255, 0),
        TRUE);
}