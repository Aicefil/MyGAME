#include "Map.h"
#include <cstdlib>
#include <ctime>

// コンストラクタ
Map::Map()
{
    srand((unsigned int)time(NULL));

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            // 外周は壁
            if (x == 0 || y == 0 ||
                x == WIDTH - 1 || y == HEIGHT - 1)
            {
                tiles[y][x] = 1;  // 壁
            }
            else
            {
                tiles[y][x] = 0;  // 床
            }
        }
    }

    // ===== 箱ランダム生成 =====
    for (int y = 2; y < HEIGHT - 2; y++)
    {
        for (int x = 2; x < WIDTH - 2; x++)
        {
            // プレイヤー中央付近は空ける
            if (x >= WIDTH / 2 - 3 && x <= WIDTH / 2 + 3 &&
                y >= HEIGHT / 2 - 3 && y <= HEIGHT / 2 + 3)
                continue;

            // 確率（5%くらいに下げる）
            if (rand() % 100 < 5)
            {
                bool nearBox = false;

                // 周囲2マスチェック
                for (int oy = -2; oy <= 2; oy++)
                {
                    for (int ox = -2; ox <= 2; ox++)
                    {
                        if (tiles[y + oy][x + ox] == 2)
                        {
                            nearBox = true;
                            break;
                        }
                    }
                    if (nearBox) break;
                }

                if (!nearBox)
                {
                    tiles[y][x] = 2;
                }
            }
        }
    }
}

// 描画
void Map::Draw()
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            int px = x * TILE;
            int py = y * TILE;

            if (tiles[y][x] == 1)
            {
                // 壁
                DrawBox(px, py,
                    px + TILE, py + TILE,
                    GetColor(120, 120, 120), TRUE);
            }
            else if (tiles[y][x] == 2)
            {
                int margin = 2;  // 箱を小さくする余白
                // 箱
                int left = px + margin;
                int right = px + TILE - margin;
                int top = py + margin;
                int bottom = py + TILE - margin;

                // ① 影
                DrawBox(
                    left + 2,
                    bottom - 3,
                    right + 2,
                    bottom,
                    GetColor(0, 0, 0),
                    TRUE
                );

                // ② 箱本体
                DrawBox(
                    left,
                    top,
                    right,
                    bottom,
                    GetColor(160, 110, 60),
                    TRUE
                );

                // ③ 上面（ハイライト）
                DrawLine(
                    left,
                    top,
                    right,
                    top,
                    GetColor(210, 170, 110)
                );

                DrawLine(
                    left,
                    top,
                    left,
                    bottom,
                    GetColor(210, 170, 110)
                );
            }
            else
            {
                // 床
                DrawBox(px, py,
                    px + TILE, py + TILE,
                    GetColor(40, 40, 40), TRUE);
            }
        }
    }
}


// 壁判定
bool Map::IsWallByWorld(float wx, float wy) const
{
    int tx = (int)wx / TILE;
    int ty = (int)wy / TILE;

    if (tx < 0 || ty < 0 ||
        tx >= WIDTH || ty >= HEIGHT)
        return true;

    // 壁(1)と箱(2)は通れない
    return tiles[ty][tx] != 0;
}