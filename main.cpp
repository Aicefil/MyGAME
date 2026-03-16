#include "DxLib.h"
#include "Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    ChangeWindowMode(TRUE);

    SetGraphMode(1280, 720, 32);

    DxLib_Init();

    SetDrawScreen(DX_SCREEN_BACK);

    gGame = new Game();

    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();

        gGame->Update();
        gGame->Draw();

        ScreenFlip();
    }

    DxLib_End();

    return 0;
}