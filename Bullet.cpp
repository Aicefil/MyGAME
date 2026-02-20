#include "Bullet.h"
#include <cmath>

// コンストラクタ
Bullet::Bullet(float sx, float sy, float angle)
{
    x = sx;
    y = sy;

    float speed = 7.0f;

    vx = cosf(angle) * speed;
    vy = sinf(angle) * speed;

    dead = false;   // 念のため初期化
}

// 移動処理
void Bullet::Update()
{
    x += vx;
    y += vy;
}

// 描画
void Bullet::Draw()
{
    DrawCircle(
        (int)x,
        (int)y,
        4,
        GetColor(255, 220, 120),
        TRUE
    );
}