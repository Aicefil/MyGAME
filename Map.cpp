#include "Map.h"
#include "EnemyManager.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

// コンストラクタ
Map::Map()
{
    srand((unsigned int)time(NULL));

    stageImg = LoadGraph("assets/stage.png");

    // 床
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            tiles[y][x] = 0;
        }
    }

    // 箱生成
    for (int y = 2; y < HEIGHT - 2; y++)
    {
        for (int x = 2; x < WIDTH - 2; x++)
        {
            if (x >= WIDTH / 2 - 3 && x <= WIDTH / 2 + 3 &&
                y >= HEIGHT / 2 - 3 && y <= HEIGHT / 2 + 3)
                continue;

            if (rand() % 100 < 30)
            {
                bool nearBox = false;

                for (int oy = -2; oy <= 2; oy++)
                {
                    for (int ox = -2; ox <= 2; ox++)
                    {
                        if (tiles[y + oy][x + ox] != 0)
                        {
                            nearBox = true;
                            break;
                        }
                    }
                    if (nearBox) break;
                }

                if (!nearBox)
                {
                    if (rand() % 100 < 20)
                        tiles[y][x] = 3; // 爆発箱
                    else
                        tiles[y][x] = 2; // 通常箱
                }
            }
        }
    }
}

// 背景
void Map::DrawBackground()
{
    DrawGraph(0, 0, stageImg, TRUE);
}

// 障害物
void Map::DrawObstacles()
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (tiles[y][x] == 2 || tiles[y][x] == 3)
            {
                int px = x * TILE;
                int py = y * TILE;

                int margin = 1;

                int left = px + margin;
                int right = px + TILE - margin;
                int top = py + margin;
                int bottom = py + TILE - margin;

                // ★爆発箱だけ大きくする
                if (tiles[y][x] == 3)
                {
                    int expand = 4;  // ← 調整ポイント（おすすめ4〜6）
                    left -= expand;
                    right += expand;
                    top -= expand;
                    bottom += expand;
                }

                // 影
                DrawBox(left + 2, bottom - 3, right + 2, bottom, GetColor(0, 0, 0), TRUE);

                // 本体
                if (tiles[y][x] == 2)
                {
                    DrawBox(left, top, right, bottom, GetColor(160, 110, 60), TRUE);
                }
                else
                {
                    DrawBox(left, top, right, bottom, GetColor(200, 60, 60), TRUE);
                    DrawString(left + 6, top + 4, "!", GetColor(255, 255, 255));
                }

                // ハイライト
                DrawLine(left, top, right, top, GetColor(210, 170, 110));
                DrawLine(left, top, left, bottom, GetColor(210, 170, 110));
            }
        }
    }
}

// ===== 爆発 =====
void Map::Explode(int tx, int ty)
{
    tiles[ty][tx] = 0;

    float cx = tx * TILE + TILE / 2.0f;
    float cy = ty * TILE + TILE / 2.0f;

    Explosion e;
    e.x = cx;
    e.y = cy;
    e.radius = 0;
    e.timer = 30;

    // ★火花生成
    for (int i = 0; i < 20; i++)
    {
        float angle = (float)(rand() % 360) * 3.14159f / 180.0f;
        float speed = (rand() % 50) / 10.0f + 2.0f;

        Particle p;
        p.x = cx;
        p.y = cy;
        p.vx = cosf(angle) * speed;
        p.vy = sinf(angle) * speed;
        p.timer = 20;

        e.particles.push_back(p);
    }

    explosions.push_back(e);

    float radius = 120.0f;

    for (auto& enemy : EnemyManager::enemies)
    {
        float dx = enemy.x - cx;
        float dy = enemy.y - cy;

        if (dx * dx + dy * dy < radius * radius)
        {
            enemy.Damage(5);
            enemy.isStuck = false;
        }
    }
}

// ===== エフェクト更新 =====
void Map::UpdateEffects()
{
    for (auto& e : explosions)
    {
        e.timer--;
        e.radius += 10.0f;

        // ★パーティクル更新
        for (auto& p : e.particles)
        {
            p.x += p.vx;
            p.y += p.vy;
            p.vy += 0.2f; // 重力っぽく
            p.timer--;
        }
    }

    explosions.erase(
        std::remove_if(
            explosions.begin(),
            explosions.end(),
            [](Explosion& e)
            {
                return e.timer <= 0;
            }),
        explosions.end());
}

// ===== エフェクト描画 =====
void Map::DrawEffects()
{
    for (auto& e : explosions)
    {
        int alpha = e.timer * 8;
        if (alpha > 255) alpha = 255;

        SetDrawBlendMode(DX_BLENDMODE_ADD, alpha);

        // ★中心爆発（塗りつぶし）
        DrawCircle((int)e.x, (int)e.y, (int)(e.radius * 0.4f), GetColor(255, 150, 50), TRUE);

        // ★外リング
        DrawCircle((int)e.x, (int)e.y, (int)e.radius, GetColor(255, 220, 100), FALSE);

        // ★さらに外の薄いリング
        DrawCircle((int)e.x, (int)e.y, (int)(e.radius * 1.3f), GetColor(255, 100, 50), FALSE);

        // ★火花
        for (auto& p : e.particles)
        {
            if (p.timer > 0)
            {
                DrawCircle((int)p.x, (int)p.y, 3, GetColor(255, 200, 50), TRUE);
            }
        }

        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}

void Map::ResetMap()
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            tiles[y][x] = 0;
        }
    }

    for (int y = 2; y < HEIGHT - 2; y++)
    {
        for (int x = 2; x < WIDTH - 2; x++)
        {
            if (x >= WIDTH / 2 - 3 && x <= WIDTH / 2 + 3 &&
                y >= HEIGHT / 2 - 3 && y <= HEIGHT / 2 + 3)
                continue;

            if (rand() % 100 < 30)
            {
                bool nearBox = false;

                for (int oy = -2; oy <= 2; oy++)
                {
                    for (int ox = -2; ox <= 2; ox++)
                    {
                        if (tiles[y + oy][x + ox] != 0)
                        {
                            nearBox = true;
                            break;
                        }
                    }
                    if (nearBox) break;
                }

                if (!nearBox)
                {
                    if (rand() % 100 < 20)
                        tiles[y][x] = 3;
                    else
                        tiles[y][x] = 2;
                }
            }
        }
    }
}

void Map::ResetEffects()
{
    explosions.clear();
}

// ===== 壁判定 =====
bool Map::IsWallByWorld(float wx, float wy) const
{
    int tx = (int)(wx / TILE);
    int ty = (int)(wy / TILE);

    if (tx < 0 || ty < 0 ||
        tx >= WIDTH || ty >= HEIGHT)
        return true;

    return tiles[ty][tx] != 0;
}

bool Map::IsWallForEnemy(float wx, float wy) const
{
    int tx = (int)(wx / TILE);
    int ty = (int)(wy / TILE);

    // 画面外は壁じゃない
    if (tx < 0 || ty < 0 ||
        tx >= WIDTH || ty >= HEIGHT)
        return false;

    return tiles[ty][tx] != 0;
}