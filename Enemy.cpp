#include "Enemy.h"
#include <cmath>

Enemy::Enemy(float _x, float _y)
{
    x = _x;
    y = _y;
}

void Enemy::Update(float px, float py)
{
    // プレイヤー方向ベクトル
    float dx = px - x;
    float dy = py - y;

    float len = sqrtf(dx * dx + dy * dy);

    // 正規化して移動
    if (len > 0)
    {
        x += (dx / len) * speed;
        y += (dy / len) * speed;
    }
}


void Enemy::Draw()
{
    // 影
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
    DrawOval(x, y + 12, 14, 6,
        GetColor(0, 0, 0), TRUE);
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