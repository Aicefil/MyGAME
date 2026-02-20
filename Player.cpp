#include "Player.h"
#include "Map.h"
#include "EnemyManager.h"
#include "BulletManager.h"
#include "Camera.h"

// コンストラクタ
Player::Player(float _x, float _y)
{
    x = _x;
    y = _y;

    img = LoadGraph("assets/player.png");

    hp = maxHp;
    life = 2;
    invincibleTime = 0;
}

void Player::Damage(int dmg)
{
    if (invincibleTime > 0 || isDead) return;

    hp -= dmg;
    invincibleTime = 120;

    if (hp <= 0)
    {
        life--;

        if (life > 0)
        {
            hp = maxHp;
        }
        else
        {
            isDead = true;
        }
    }
}

void Player::Update(const Map& map)
{
    if (isDead) return;

    if (invincibleTime > 0)
        invincibleTime--;

    float prevX = x;
    float prevY = y;

    float speed = 3.0f;

    if (CheckHitKey(KEY_INPUT_W)) y -= speed;
    if (CheckHitKey(KEY_INPUT_S)) y += speed;
    if (CheckHitKey(KEY_INPUT_A)) x -= speed;
    if (CheckHitKey(KEY_INPUT_D)) x += speed;

    // Mapは外部から渡されたものを使う
    if (map.IsWallByWorld(x, y))
    {
        x = prevX;
        y = prevY;
    }

    // 照準
    int mx, my;
    GetMousePoint(&mx, &my);
    angle = atan2f(my - y, mx - x);

    // 射撃
    static int shootDelay = 0;
    if (shootDelay > 0) shootDelay--;

    if ((GetMouseInput() & MOUSE_INPUT_LEFT) &&
        ammo > 0 &&
        shootDelay <= 0)
    {
        BulletManager::Spawn(x, y, angle);
        ammo--;
        shootDelay = 60;
    }

    // リロード
    if (ammo == 0)
    {
        reloadTime++;
        if (reloadTime > 120)
        {
            ammo = 6;
            reloadTime = 0;
        }
    }

    // 敵接触
    for (auto& e : EnemyManager::enemies)
    {
        float dx = x - e.x;
        float dy = y - e.y;

        if (dx * dx + dy * dy < 16 * 16)
        {
            Damage(1);
            break;
        }
    }
}

void Player::Draw()
{
    if (isDead)
    {
        DrawString(550, 350, "GAME OVER",
            GetColor(255, 0, 0));
        return;
    }

    if (invincibleTime > 0)
    {
        if ((invincibleTime / 5) % 2 == 0)
            return;
    }

    // 影
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
    DrawOval(x, y + 10, 16, 6,
        GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // 本体
    DrawRotaGraphF(
        x,
        y,
        0.5f,
        angle,
        img,
        TRUE
    );

    // 残弾表示 
    int bulletSpacing = 10;

    for (int i = 0; i < ammo; i++)
    {
        DrawCircle(
            (int)x - (ammo * bulletSpacing) / 2 + i * bulletSpacing,
            (int)y - 40,   // キャラの上
            3,
            GetColor(255, 220, 120),
            TRUE
        );
    }

    // リロード表示 
    if (ammo == 0)
    {
        // 点滅
        if ((reloadTime / 10) % 2 == 0)
        {
            DrawFormatString(
                (int)x - 40,
                (int)y - 70,
                GetColor(255, 80, 80),
                "Reloading..."
            );
        }
    }

    DrawFormatString(20, 20,
        GetColor(255, 255, 255),
        "HP: %d  Life: %d",
        hp, life);
}