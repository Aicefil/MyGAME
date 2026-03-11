#include "Player.h"
#include "Map.h"
#include "EnemyManager.h"
#include "BulletManager.h"

// コンストラクタ
Player::Player(float _x, float _y)
{
    x = _x;
    y = _y;

    img = LoadGraph("assets/player.png");

    hp = maxHp;
    life = 2;
    invincibleTime = 0;
    isDead = false;
    ammo = 6;
    reloadTime = 0;
}

// ダメージ処理
void Player::Damage(int dmg)
{
    if (isDead) return;          // 死亡時は処理なし
    if (invincibleTime > 0) return; // 無敵中は処理なし

    hp -= dmg;
    invincibleTime = 120; // 無敵フレームを設定

    if (hp <= 0)
    {
        life--;
        if (life > 0)
        {
            hp = maxHp; // 残りライフがあればHPリセット
        }
        else
        {
            isDead = true;
            hp = 0;
        }
    }
}

// 更新処理
void Player::Update(const Map& map)
{
    if (isDead) return;

    // 無敵時間のカウントダウン
    if (invincibleTime > 0)
        invincibleTime--;

    // 移動
    float speed = 1.5f;
    if (CheckHitKey(KEY_INPUT_W)) y -= speed;
    if (CheckHitKey(KEY_INPUT_S)) y += speed;
    if (CheckHitKey(KEY_INPUT_A)) x -= speed;
    if (CheckHitKey(KEY_INPUT_D)) x += speed;

    // 画面外制限
    if (x < 20) x = 20;
    if (y < 20) y = 20;
    if (x > 1260) x = 1260;
    if (y > 700) y = 700;

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
        shootDelay = 80;
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

    // 敵接触判定
    for (auto& e : EnemyManager::enemies)
    {
        // 中心座標同士で判定
        float dx = x - e.x;
        float dy = y - e.y;
        float distanceSq = dx * dx + dy * dy;
        float hitRadius = 25.0f;

        if (distanceSq < hitRadius * hitRadius)
        {
            Damage(1);
            break; // 1回ダメージを受けたらループ抜け
        }
    }
}

// 描画処理
void Player::Draw()
{
    if (isDead)
    {
        DrawString(550, 350, "GAME OVER", GetColor(255, 0, 0));
        return;
    }

    // 無敵時間点滅処理
    if (invincibleTime > 0 && (invincibleTime / 5) % 2 == 0)
        return;

    // 影
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
    DrawOval((int)x, (int)y + 10, 18, 6, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // プレイヤー本体（回転なし）
    DrawRotaGraphF(x, y, 0.55f, 0.0f, img, TRUE);

    // 残弾表示
    int bulletSpacing = 12;
    for (int i = 0; i < ammo; i++)
    {
        DrawCircle((int)x - (ammo * bulletSpacing) / 2 + i * bulletSpacing,
            (int)y - 40,
            4,
            GetColor(255, 220, 120),
            TRUE);
    }

    // リロード表示
    if (ammo == 0 && (reloadTime / 10) % 2 == 0)
    {
        DrawFormatString((int)x - 40, (int)y - 60, GetColor(255, 80, 80), "Reloading...");
    }

    // HP表示
    DrawFormatString(20, 20, GetColor(255, 255, 255), "HP: %d  Life: %d", hp, life);
}