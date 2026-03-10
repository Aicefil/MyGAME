#pragma once
#include <vector>
#include "Enemy.h"

class EnemyManager
{
public:
    static std::vector<Enemy> enemies;

    static void Init(int count, int wave);
    static void UpdateAll(float playerX, float playerY, const Map& map);

    // ƒJƒƒ‰•t‚«
    static void DrawAll();
};