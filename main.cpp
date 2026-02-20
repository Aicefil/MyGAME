#include "DxLib.h"
#include "Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    DxLib_Init();
    SetDrawScreen(DX_SCREEN_BACK);

    Game game;
    game.Init();

    while (ProcessMessage() == 0 && ScreenFlip() == 0 && ClearDrawScreen() == 0) {
        game.Update(); // ëSèàóù
        game.Draw();   // âÊñ ï`âÊ
    }

    DxLib_End();
    return 0;
}
