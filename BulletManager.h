#pragma once
#include <vector>
#include "Bullet.h"
#include "Map.h"       

class BulletManager
{
public:
    static std::vector<Bullet> bullets;

    static void Spawn(float x, float y, float angle);

    static void UpdateAll(Map& map);

    static void DrawAll();
};