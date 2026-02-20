#pragma once
#include <vector>
#include "Enemy.h"
#include "Camera.h"

class EnemyManager
{
public:
    static std::vector<Enemy> enemies;

    static void Init(int count);
    static void UpdateAll(float playerX, float playerY);

    // ƒJƒƒ‰•t‚«
    static void DrawAll();
};