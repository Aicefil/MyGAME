#include "EnemyManager.h"
#include <algorithm>
#include <cstdlib>

std::vector<Enemy> EnemyManager::enemies;


// “G¶¬
void EnemyManager::Init(int count, int wave)
{
    enemies.clear();

    for (int i = 0; i < count; i++)
    {
        float x, y;

        int side = rand() % 4;

        if (side == 0) // ã
        {
            x = rand() % 1200 + 40;
            y = 0;
        }
        else if (side == 1) // ‰º
        {
            x = rand() % 1200 + 40;
            y = 720;
        }
        else if (side == 2) // ¶
        {
            x = 0;
            y = rand() % 640 + 40;
        }
        else // ‰E
        {
            x = 1280;
            y = rand() % 640 + 40;
        }

        enemies.emplace_back(x, y, wave);
    }
}


// XV
void EnemyManager::UpdateAll(float px, float py, const Map& map)
{
    for (auto& e : enemies)
    {
        e.Update(px, py, map);
    }

    enemies.erase(
        std::remove_if(
            enemies.begin(),
            enemies.end(),
            [](Enemy& e)
            {
                return e.isDead;
            }),
        enemies.end());
}


// •`‰æ
void EnemyManager::DrawAll()
{
    for (auto& e : enemies)
    {
        e.Draw();
    }
}