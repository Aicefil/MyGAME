#pragma once
#include <DxLib.h>

// ===============================
// 弾クラス
// ・移動のみ担当
// ・当たり判定はManager側
// ===============================
class Bullet
{
public:
    float x, y;      // 位置
    float vx, vy;    // 速度

    bool dead = false;      // 消滅フラグ

    Bullet(float sx, float sy, float angle);

    void Update();
    void Draw();
};
