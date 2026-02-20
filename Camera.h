#pragma once

class Map; // 前方宣言

class Camera
{
public:
    float x;
    float y;

    Camera();

    // マップ基準で更新
    void Update(const Map& map);
};