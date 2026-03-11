#include "Map.h"
#include <cstdlib>
#include <ctime>

// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
Map::Map()
{
    srand((unsigned int)time(NULL));

    // ”wŒi‰æ‘œ
    stageImg = LoadGraph("assets/stage.png");

    // °
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            tiles[y][x] = 0;
        }
    }

    // ” ¶¬
    for (int y = 2; y < HEIGHT - 2; y++)
    {
        for (int x = 2; x < WIDTH - 2; x++)
        {
            if (x >= WIDTH / 2 - 3 && x <= WIDTH / 2 + 3 &&
                y >= HEIGHT / 2 - 3 && y <= HEIGHT / 2 + 3)
                continue;

            if (rand() % 100 < 30)
            {
                bool nearBox = false;

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
                    tiles[y][x] = 2;
            }
        }
    }
}

// ”wŒi‚¾‚¯•`‰æ
void Map::DrawBackground()
{
    DrawGraph(0, 0, stageImg, TRUE);
}

// áŠQ•¨‚¾‚¯•`‰æ
void Map::DrawObstacles()
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (tiles[y][x] == 2)
            {
                int px = x * TILE;
                int py = y * TILE;
                int margin = 1;

                int left = px + margin;
                int right = px + TILE - margin;
                int top = py + margin;
                int bottom = py + TILE - margin;

                // ‰e
                DrawBox(left + 2, bottom - 3, right + 2, bottom, GetColor(0, 0, 0), TRUE);

                // ” 
                DrawBox(left, top, right, bottom, GetColor(160, 110, 60), TRUE);

                // ƒnƒCƒ‰ƒCƒg
                DrawLine(left, top, right, top, GetColor(210, 170, 110));
                DrawLine(left, top, left, bottom, GetColor(210, 170, 110));
            }
        }
    }
}


// ===== •Ç”»’è =====
bool Map::IsWallByWorld(float wx, float wy) const
{
    int tx = (int)wx / TILE;
    int ty = (int)wy / TILE;

    if (tx < 0 || ty < 0 ||
        tx >= WIDTH || ty >= HEIGHT)
        return true;

    // •Ç(1)‚Æ” (2)‚Í’Ê‚ê‚È‚¢
    return tiles[ty][tx] != 0;
}