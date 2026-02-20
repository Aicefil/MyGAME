#include "Camera.h"
#include "Map.h"

Camera::Camera()
{
    x = 0;
    y = 0;
}

// マップ全体の中央を表示する固定カメラ
void Camera::Update(const Map& map)
{
    const float screenW = 1280.0f;
    const float screenH = 720.0f;

    // ★ タイル数 × タイルサイズ でピクセル計算
    float mapPixelW = Map::WIDTH * Map::TILE;
    float mapPixelH = Map::HEIGHT * Map::TILE;

    x = (mapPixelW - screenW) * 0.5f;
    y = (mapPixelH - screenH) * 0.5f;

    if (x < 0) x = 0;
    if (y < 0) y = 0;
}