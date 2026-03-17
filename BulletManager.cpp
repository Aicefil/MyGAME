#include "BulletManager.h"
#include "EnemyManager.h"
#include "Game.h"
#include <algorithm>
#include "Map.h"

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

        // ===== 爆発箱チェック =====
        int tx = (int)(b.x / Map::TILE);
        int ty = (int)(b.y / Map::TILE);

        if (tx >= 0 && ty >= 0 && tx < Map::WIDTH && ty < Map::HEIGHT)
        {
            if (map.tiles[ty][tx] == 3)
            {
                map.Explode(tx, ty);
                b.dead = true;
                continue;
            }
        }

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

                if (gGame)
                    gGame->hitStopTimer = 3;

                break;
            }
        }

        // 画面外
        if (b.x < 0 || b.x > 1280 ||
            b.y < 0 || b.y > 720)
        {
            b.dead = true;
        }
    }

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

void BulletManager::Clear()
{
    bullets.clear();
}