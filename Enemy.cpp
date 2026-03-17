#include "Enemy.h"
#include <cmath>
#include "EnemyManager.h"
#include "Map.h"   
#include "Game.h"
extern Game* gGame;

Enemy::Enemy(float _x, float _y, int wave)
{
    x = _x;
    y = _y;

    speed = 0.3f + wave * 0.2f;

    isDead = false;
    isStuck = false;
    stuckTx = -1;
    stuckTy = -1;
}

void Enemy::Update(float px, float py, const Map& map)
{
    const float radius = 14.0f;

    // ===== îSíÖÇµÇƒÇÈèÍçá =====
    if (isStuck)
    {
        float cx = stuckTx * Map::TILE + Map::TILE / 2.0f;
        float cy = stuckTy * Map::TILE + Map::TILE / 2.0f;

        float dx = x - cx;
        float dy = y - cy;
        float angle = atan2f(dy, dx);

        float stickRadius = Map::TILE / 2.0f + radius;
        x = cx + cosf(angle) * stickRadius;
        y = cy + sinf(angle) * stickRadius;

        return;
    }

    // ===== ÉvÉåÉCÉÑÅ[í«îˆ =====
    float dx = px - x;
    float dy = py - y;
    float len = sqrtf(dx * dx + dy * dy);
    if (len == 0) return;

    float moveX = (dx / len) * speed;
    float moveY = (dy / len) * speed;
    float nextX = x + moveX;
    float nextY = y + moveY;

    // ===== ï«îªíËÅiääÇÈî≈Åj =====
    if (!map.IsWallForEnemy(nextX + radius, y) &&
        !map.IsWallForEnemy(nextX - radius, y))
    {
        x = nextX;
    }

    if (!map.IsWallForEnemy(x, nextY + radius) &&
        !map.IsWallForEnemy(x, nextY - radius))
    {
        y = nextY;
    }

    // ===== ïÅí ÇÃî†Åi2ÅjÇ∆ÇÃâ~îªíË =====
    int cxTile = (int)(x / Map::TILE);
    int cyTile = (int)(y / Map::TILE);

    for (int oy = -1; oy <= 1; oy++)
    {
        for (int ox = -1; ox <= 1; ox++)
        {
            int tx = cxTile + ox;
            int ty = cyTile + oy;

            if (tx < 0 || ty < 0 || tx >= Map::WIDTH || ty >= Map::HEIGHT) continue;

            if (map.tiles[ty][tx] == 2) // ïÅí ÇÃî†
            {
                float bx = tx * Map::TILE + Map::TILE / 2.0f;
                float by = ty * Map::TILE + Map::TILE / 2.0f;

                float dxBox = x - bx;
                float dyBox = y - by;
                float dist = sqrtf(dxBox * dxBox + dyBox * dyBox);
                float minDist = Map::TILE / 2.0f + radius;

                if (dist < minDist && dist > 0.0f)
                {
                    float overlap = minDist - dist;
                    x += (dxBox / dist) * overlap;
                    y += (dyBox / dist) * overlap;
                }
            }
        }
    }

    // ===== îöî≠î†Åi3ÅjÉ`ÉFÉbÉN =====
    for (int oy = -1; oy <= 1; oy++)
    {
        for (int ox = -1; ox <= 1; ox++)
        {
            int tx = cxTile + ox;
            int ty = cyTile + oy;

            if (tx < 0 || ty < 0 || tx >= Map::WIDTH || ty >= Map::HEIGHT) continue;

            if (map.tiles[ty][tx] == 3) // îöî≠î†
            {
                float bx = tx * Map::TILE + Map::TILE / 2.0f;
                float by = ty * Map::TILE + Map::TILE / 2.0f;

                float dxBox = x - bx;
                float dyBox = y - by;
                float dist2 = dxBox * dxBox + dyBox * dyBox;

                if (dist2 < 1600.0f) // îºåa40px
                {
                    isStuck = true;
                    stuckTx = tx;
                    stuckTy = ty;
                    return;
                }
            }
        }
    }

    // ===== ìGìØémÇÃâüÇµçáÇ¢ =====
    for (auto& other : EnemyManager::enemies)
    {
        if (&other == this) continue;

        float dxOther = x - other.x;
        float dyOther = y - other.y;
        float dist = sqrtf(dxOther * dxOther + dyOther * dyOther);
        float minDist = 28.0f;

        if (dist > 0 && dist < minDist)
        {
            float push = (minDist - dist) * 0.05f;
            x += (dxOther / dist) * push;
            y += (dyOther / dist) * push;
        }
    }

    // ===== âÊñ äOñhé~ =====
    float minX = radius;
    float maxX = Map::WIDTH * Map::TILE - radius;
    float minY = radius;
    float maxY = Map::HEIGHT * Map::TILE - radius;

    if (x < minX) x = minX;
    if (x > maxX) x = maxX;
    if (y < minY) y = minY;
    if (y > maxY) y = maxY;
}

// ===== É_ÉÅÅ[ÉW =====
void Enemy::Damage(int dmg)
{
    if (!isDead) // 2èdâ¡éZñhé~
    {
        isDead = true;
        if (gGame)
        {
            gGame->AddScore(100); // ì|ÇµÇΩÇÁ100ì_â¡éZ
        }
    }
}

// ===== ï`âÊ =====
void Enemy::Draw()
{
    int bodyColor = isStuck ? GetColor(255, 80, 80) : GetColor(200, 50, 50);

    // âe
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
    DrawOval((int)x, (int)y + 12, 14, 6, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // ñ{ëÃ
    DrawCircle((int)x, (int)y, 14, bodyColor, TRUE);
}