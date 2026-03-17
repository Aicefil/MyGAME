#pragma once
#include <DxLib.h>
#include <vector>

class Map
{
public:
    static const int WIDTH = 40;
    static const int HEIGHT = 22;
    static const int TILE = 32;

    int tiles[HEIGHT][WIDTH];

    Map();

    void DrawBackground();
    void DrawObstacles();

    bool IsWallByWorld(float wx, float wy) const;
    bool IsWallForEnemy(float wx, float wy) const;

    void Explode(int tx, int ty);

    // エフェクト
    void UpdateEffects();
    void DrawEffects();

    void ResetMap();
    void ResetEffects();

private:
    int stageImg;

    struct Particle
    {
        float x = 0.0f;
        float y = 0.0f;
        float vx = 0.0f;
        float vy = 0.0f;
        int timer = 0;
    };

    // ===== 爆発エフェクト =====
    struct Explosion
    {
        float x = 0.0f;
        float y = 0.0f;
        float radius = 0.0f;
        int timer = 0;

        std::vector<Particle> particles;
    };

    std::vector<Explosion> explosions;
};