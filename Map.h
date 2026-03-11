#pragma once
#include <DxLib.h>

class Map
{
public:
    Map();                  // コンストラクタ

    void DrawBackground();  // 背景だけ描画
    void DrawObstacles();   // 障害物だけ描画

    bool IsWallByWorld(float wx, float wy) const;

private:
    static const int WIDTH = 20;   // 横タイル数
    static const int HEIGHT = 15;   // 縦タイル数
    static const int TILE = 64;   // タイル1マスのサイズ（px）

    int tiles[HEIGHT][WIDTH];        // マップデータ（0:床, 2:箱など）
    int stageImg;                    // 背景画像ID
};