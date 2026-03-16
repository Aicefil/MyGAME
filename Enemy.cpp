#include "Enemy.h"
#include <cmath>
#include "EnemyManager.h"

Enemy::Enemy(float _x, float _y, int wave)
{
    x = _x;
    y = _y;

    // ウェーブで速度アップ
    speed = 0.3f + wave * 0.2f;
}

void Enemy::Update(float px, float py, const Map& map)
{
    float dx = px - x;
    float dy = py - y;

    float len = sqrtf(dx * dx + dy * dy);

    float moveX = (dx / len) * speed;
    float moveY = (dy / len) * speed;

    // 壁無視で移動
    x += moveX;
    y += moveY;

    // 敵同士の押し合い
    for (auto& other : EnemyManager::enemies)
    {
        if (&other == this) continue;

        float dx = x - other.x;
        float dy = y - other.y;

        float dist = sqrtf(dx * dx + dy * dy);

        float minDist = 28.0f;

        if (dist > 0 && dist < minDist)
        {
            float push = (minDist - dist) * 0.05f;

            x += (dx / dist) * push;
            y += (dy / dist) * push;
        }
    }
}

void Enemy::Draw()
{
    // 影
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);

    DrawOval(
        (int)x,
        (int)y + 12,
        14,
        6,
        GetColor(0, 0, 0),
        TRUE
    );

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // 本体
    DrawCircle(
        (int)x,
        (int)y,
        14,
        GetColor(200, 50, 50),
        TRUE
    );
}