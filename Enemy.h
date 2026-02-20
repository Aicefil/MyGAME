#pragma once
#include <DxLib.h>
#include "Camera.h"

// ===============================
// 敵クラス
// ・プレイヤーを追尾
// ・死亡フラグを持つ
// ===============================
class Enemy
{
public:
    float x, y;
    float speed = 1.0f;

    bool isDead = false;

    Enemy(float _x, float _y);

    // プレイヤー座標を受け取り追尾
    void Update(float px, float py);

    void Draw();
};
