#pragma once
#include "DxLib.h"

class Map
{
public:
    static const int TILE = 64;
    static const int WIDTH = 20;   
    static const int HEIGHT = 11;  

    int tiles[HEIGHT][WIDTH];

    Map();

    bool IsWallByWorld(float wx, float wy) const;
    void Draw();
};