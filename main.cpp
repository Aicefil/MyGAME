#include "DxLib.h"
#include "Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    ChangeWindowMode(TRUE);
    SetGraphMode(1280, 720, 32);

    DxLib_Init();
    SetDrawScreen(DX_SCREEN_BACK);

    Game game;
    game.Init();

    while (ProcessMessage() == 0)
    {
        game.Update();

        ClearDrawScreen();
        game.Draw();

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}