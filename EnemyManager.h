#pragma once
#include <vector>
#include "Enemy.h"

class EnemyManager
{
public:
    static std::vector<Enemy> enemies;
    static int remainingToSpawn; // このWaveでこれから出すべき数
    static int spawnTimer;       // 出現間隔を測るタイマー

    static void Init(int count, int wave);
    static void UpdateAll(float playerX, float playerY, const Map& map);

    // カメラ付き
    static void DrawAll();
};