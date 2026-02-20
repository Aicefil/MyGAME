#include "BulletManager.h"
#include "EnemyManager.h"
#include "Game.h"
#include <algorithm>

std::vector<Bullet> BulletManager::bullets;

void BulletManager::Spawn(float x, float y, float angle)
{
    bullets.emplace_back(x, y, angle);
}

void BulletManager::UpdateAll(Map& map)
{
    for (auto& b : bullets)
    {
        b.Update();

        // 壁ヒット 
        if (map.IsWallByWorld(b.x, b.y))
        {
            b.dead = true;
            continue;
        }

        // 敵ヒット
        for (auto& e : EnemyManager::enemies)
        {
            if (e.isDead) continue;

            float dx = b.x - e.x;
            float dy = b.y - e.y;

            if (dx * dx + dy * dy < 14 * 14)
            {
                e.isDead = true;
                b.dead = true;

                // ヒットストップ
                if (gGame)
                    gGame->hitStopTimer = 3;

                break;  // 敵ループ抜ける
            }
        }

        // 画面外チェック
        if (b.x < 0 || b.x > 1280 ||
            b.y < 0 || b.y > 720)
        {
            b.dead = true;
        }
    }

    // ===== 削除処理 =====
    bullets.erase(
        std::remove_if(
            bullets.begin(),
            bullets.end(),
            [](Bullet& b) { return b.dead; }),
        bullets.end());
}

void BulletManager::DrawAll()
{
    for (auto& b : bullets)
        b.Draw();
}