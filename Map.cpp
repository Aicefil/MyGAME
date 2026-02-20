#include "Map.h"

Map::Map()
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (x == 0 || y == 0 ||
                x == WIDTH - 1 || y == HEIGHT - 1)
            {
                tiles[y][x] = 1;  // •Ç
            }
            else
            {
                tiles[y][x] = 0;  // °
            }
        }
    }
}

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
                DrawBox(px, py,
                    px + TILE, py + TILE,
                    GetColor(120, 120, 120), TRUE);
            }
            else
            {
                DrawBox(px, py,
                    px + TILE, py + TILE,
                    GetColor(40, 40, 40), TRUE);
            }
        }
    }
}

bool Map::IsWallByWorld(float wx, float wy) const
{
    int tx = (int)wx / TILE;
    int ty = (int)wy / TILE;

    if (tx < 0 || ty < 0 ||
        tx >= WIDTH || ty >= HEIGHT)
        return true;

    return tiles[ty][tx] == 1;
}
