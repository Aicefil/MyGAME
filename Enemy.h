#pragma once
#include <DxLib.h>
#include "Map.h"

// プレイヤーを追尾
// 死亡フラグを持つ
class Enemy
{
public:

    bool isStuck = false;
    int stuckTx, stuckTy;

    float x, y;
    float speed;

    bool isDead = false;

    Enemy(float _x, float _y, int wave);

    // プレイヤー座標を受け取り追尾
    void Update(float px, float py, const Map& map);
    void Damage(int dmg);

    void Draw();
};