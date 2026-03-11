#pragma once
#include <DxLib.h>

class Bullet
{
public:
    float x, y;      // 弾の位置
    float vx, vy;    // 弾の速度
    bool dead;       // 生存判定

    Bullet(float sx, float sy, float angle); // コンストラクタ

    void Update();  // 移動処理
    void Draw();    // 描画処理
};