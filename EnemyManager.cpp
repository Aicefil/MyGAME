#include "EnemyManager.h"
#include <algorithm>
#include <cstdlib>

std::vector<Enemy> EnemyManager::enemies;

void EnemyManager::Init(int count)
{
    enemies.clear();

    for (int i = 0; i < count; i++)
    {
        enemies.emplace_back(rand() % 1280,
            rand() % 720);
    }
}

void EnemyManager::UpdateAll(float px, float py)
{
    for (auto& e : enemies)
        e.Update(px, py);

    enemies.erase(
        std::remove_if(enemies.begin(),
            enemies.end(),
            [](Enemy& e) { return e.isDead; }),
        enemies.end());
}

void EnemyManager::DrawAll()
{
    for (auto& e : enemies)
        e.Draw();
}