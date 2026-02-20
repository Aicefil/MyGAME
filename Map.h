#pragma once
#include "DxLib.h"

class Map
{
public:
    static const int TILE = 32;
    static const int WIDTH = 40;   // 40 Å~ 32 = 1280
    static const int HEIGHT = 22;  // 22 Å~ 32 = 704

    int tiles[HEIGHT][WIDTH];

    Map();

    bool IsWallByWorld(float wx, float wy) const;
    void Draw();
};