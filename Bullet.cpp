#include "Bullet.h"
#include <cmath>

// コンストラクタ
Bullet::Bullet(float sx, float sy, float angle)
{
    x = sx;
    y = sy;

    const float speed = 7.0f;
    vx = cosf(angle) * speed;
    vy = sinf(angle) * speed;

    dead = false;
}

// 移動処理
void Bullet::Update()
{
    x += vx;
    y += vy;
}

// 描画処理（円で描画）
void Bullet::Draw()
{
    DrawCircle(
        (int)x,
        (int)y,
        4,                         // 半径
        GetColor(255, 220, 120),   // 色
        TRUE                       // 塗りつぶし
    );
}