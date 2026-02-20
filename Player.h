#pragma once
#include "DxLib.h"
#include <cmath>

class Map;
class Camera;

class Player
{
public:
    float x, y;
    float angle = 0;

    int maxHp = 5;
    int hp = 5;

    int life = 2;
    int invincibleTime = 0;

    bool isDead = false;

    int ammo = 6;
    int reloadTime = 0;

    int img;

    Player(float _x = 640, float _y = 360);

    void Update(const Map& map);        // © map‚ğó‚¯æ‚é
    void Draw();

    void Damage(int dmg);
};