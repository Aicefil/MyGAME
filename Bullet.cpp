#include "Bullet.h"
#include "EnemyManager.h"
#include "Enemy.h"
#include "Game.h"
#include <cmath>

extern Game* gGame; // Game のスコアにアクセス

Bullet::Bullet(float sx, float sy, float angle)
{
    x = sx;
    y = sy;

    const float speed = 7.0f;
    vx = cosf(angle) * speed;
    vy = sinf(angle) * speed;

    dead = false;
}

void Bullet::Update()
{
    if (dead) return;

    // 移動
    x += vx;
    y += vy;

    // 画面外に出たら消す
    if (x < 0 || x > 1280 || y < 0 || y > 720)
    {
        dead = true;
        return;
    }

    // ===== 敵との当たり判定 =====
    for (auto& enemy : EnemyManager::enemies)
    {
        if (enemy.isDead) continue;

        float dx = x - enemy.x;
        float dy = y - enemy.y;
        float dist2 = dx * dx + dy * dy;
        float radius = 14.0f + 4.0f; // 敵半径 + 弾半径

        if (dist2 < radius * radius)
        {
            enemy.Damage(1);   // ダメージ
            dead = true;       // 弾は消える
            break;
        }
    }
}

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