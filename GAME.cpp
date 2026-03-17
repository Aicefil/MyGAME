#include "DxLib.h"
#include "Game.h"
#include "EnemyManager.h"
#include "BulletManager.h"

#include <algorithm>
#include <vector>
#include <functional>
#include <string>
#include <cstdlib>  

// ---------------- ボタン描画＆クリック判定 ----------------
int DrawButton(int x, int y, const std::string& text, int fontHandle, int normalColor, int hoverColor)
{
    int mx, my;
    GetMousePoint(&mx, &my);

    int w = GetDrawStringWidthToHandle(text.c_str(), -1, fontHandle);
    int h = GetFontSizeToHandle(fontHandle);

    // マウスがボタン上にあるかチェック
    int color = (mx > x && mx < x + w && my > y && my < y + h) ? hoverColor : normalColor;
    DrawStringToHandle(x, y, text.c_str(), color, fontHandle);

    int click = GetMouseInput();
    return (mx > x && mx < x + w && my > y && my < y + h && (click & MOUSE_INPUT_LEFT)) ? 1 : 0;
}

// ---------------- グローバル変数 ----------------
Game* gGame = nullptr;
int fontStart;

// ---------------- 画像を中央に描画 ----------------
void DrawCenter(int img)
{
    int w, h;
    GetGraphSize(img, &w, &h);
    DrawGraph((1280 - w) / 2, (720 - h) / 2, img, TRUE);
}

// ---------------- コンストラクタ ----------------
Game::Game() : player(640, 360)
{
    imgTitle = LoadGraph("assets/title.png");
    imgGameOver = LoadGraph("assets/gameover.png");
    imgClear = LoadGraph("assets/clear.png");

    fontStart = CreateFontToHandle(NULL, 48, 3);

    // リザルト演出初期化
    resultAlpha = 0;
    resultScoreDisplay = 0;
    blinkTimer = 0;
    resultWaitTimer = 0;
}

// ---------------- 初期化 ----------------
void Game::Init()
{
    wave = 1;
    score = 0;
    spawnTimer = 0;
    waveTextTimer = 120;
    hitStopTimer = 0;

    waveTimeLimit = 30 * 60;
    waveTimer = waveTimeLimit;

    gameClear = false;
    player = Player(640, 360);

    EnemyManager::enemies.clear();
    EnemyManager::Init(5 * wave, wave);

    map.ResetMap();
    map.ResetEffects();
    BulletManager::Clear();

    SetMouseDispFlag(FALSE);
    state = STATE_WAVE_START;
}

// ---------------- リザルト開始 ----------------
void Game::ShowResult(const std::string& msg)
{
    state = STATE_RESULT;
    resultText = msg;
    resultAlpha = 0;
    resultScoreDisplay = 0;
    blinkTimer = 0;
    resultParticles.clear();
    resultWaitTimer = 0;
}

// ---------------- 更新 ----------------
void Game::Update()
{
    static int spacePrev = 0;
    int spaceNow = CheckHitKey(KEY_INPUT_SPACE);

    switch (state)
    {
    case STATE_TITLE:
    {
        if (DrawButton(530, 520, "ゲーム開始", fontStart, GetColor(255, 255, 255), GetColor(255, 255, 0)))
            Init();
        break;
    }

    case STATE_WAVE_START:
        waveTextTimer--;
        if (waveTextTimer <= 0)
            state = STATE_PLAYING;
        break;

    case STATE_PLAYING:
        UpdatePlaying();
        break;

    case STATE_CLEAR:
        // クリア画面3秒表示
        if (resultWaitTimer == 0) resultWaitTimer = 3 * 60;
        else
        {
            resultWaitTimer--;
            if (resultWaitTimer <= 0)
                ShowResult("GAME CLEAR");
        }
        break;

    case STATE_GAMEOVER:
        // ゲームオーバー画面3秒表示
        if (resultWaitTimer == 0) resultWaitTimer = 3 * 60;
        else
        {
            resultWaitTimer--;
            if (resultWaitTimer <= 0)
                ShowResult("GAME OVER");
        }
        break;

    case STATE_RESULT:
        UpdateResult();
        if (spaceNow && !spacePrev)
        {
            state = STATE_TITLE;
            score = 0;
        }
        break;
    }

    spacePrev = spaceNow;
}

// ---------------- WAVE中の更新 ----------------
void Game::UpdatePlaying()
{
    if (hitStopTimer > 0) { hitStopTimer--; return; }

    player.Update(map);
    BulletManager::UpdateAll(map);
    EnemyManager::UpdateAll(player.x, player.y, map);
    map.UpdateEffects();

    waveTimer--;
    bool waveAdvanced = false;

    if (waveTimer <= 0)
    {
        wave++;
        waveTimer = waveTimeLimit;
        waveAdvanced = true;
    }

    if (!waveAdvanced && EnemyManager::enemies.empty())
    {
        spawnTimer++;
        if (spawnTimer > 120)
        {
            spawnTimer = 0;
            wave++;
            waveTimer = waveTimeLimit;
            waveAdvanced = true;
        }
    }
    else spawnTimer = 0;

    if (waveAdvanced)
    {
        player = Player(640, 360);
        EnemyManager::enemies.clear();
        EnemyManager::Init(5 * wave, wave);
        map.ResetMap();
        map.ResetEffects();
        BulletManager::Clear();
        waveTextTimer = 120;

        if (wave > 3)
            state = STATE_CLEAR;  // ここでクリア画面に移行
        else
            state = STATE_WAVE_START;
    }

    if (player.isDead)
    {
        state = STATE_GAMEOVER;   // 死亡でゲームオーバー画面に移行
        return;
    }

    for (auto it = EnemyManager::enemies.begin(); it != EnemyManager::enemies.end(); )
    {
        if (it->isDead)
        {
            score += 100;
            it = EnemyManager::enemies.erase(it);
        }
        else ++it;
    }
}

// ---------------- リザルト演出更新 ----------------
void Game::UpdateResult()
{
    if (resultAlpha < 255) resultAlpha += 5;

    if (resultScoreDisplay < score) resultScoreDisplay += 5;
    if (resultScoreDisplay > score) resultScoreDisplay = score;

    if (rand() % 3 == 0)
    {
        Particle p;
        p.x = 540 + rand() % 200 - 100;
        p.y = 280 + rand() % 50 - 25;
        p.r = rand() % 3 + 1;
        p.alpha = 255;
        resultParticles.push_back(p);
    }

    for (auto it = resultParticles.begin(); it != resultParticles.end(); )
    {
        it->y -= 0.5f;
        it->alpha -= 5;
        if (it->alpha <= 0) it = resultParticles.erase(it);
        else ++it;
    }

    blinkTimer++;
    if (blinkTimer > 60) blinkTimer = 0;
}

// ---------------- 描画 ----------------
void Game::Draw()
{
    map.DrawBackground();
    map.DrawObstacles();
    map.DrawEffects();

    // スコアは常に表示
    char scoreBuf[32];
    sprintf_s(scoreBuf, sizeof(scoreBuf), "SCORE: %d", score);
    DrawString(10, 50, scoreBuf, GetColor(255, 255, 255));

    // ゲーム中だけWAVE残り時間表示
    if (state == STATE_WAVE_START || state == STATE_PLAYING)
    {
        int seconds = waveTimer / 60;
        int mins = seconds / 60;
        int secs = seconds % 60;
        char buf[16];
        sprintf_s(buf, sizeof(buf), "WAVE %d  %02d:%02d", wave, mins, secs);
        int textWidth = GetDrawStringWidthToHandle(buf, -1, fontStart);
        DrawStringToHandle((1280 - textWidth) / 2, 20, buf, GetColor(255, 255, 0), fontStart);
    }

    switch (state)
    {
    case STATE_TITLE:
        DrawCenter(imgTitle);
        if (DrawButton(530, 520, "ゲーム開始", fontStart, GetColor(255, 255, 255), GetColor(255, 255, 0)))
            Init();
        break;

    case STATE_WAVE_START:
        DrawGame();
        break;

    case STATE_PLAYING:
        DrawGame();
        break;

    case STATE_CLEAR:
        DrawCenter(imgClear);
        break;

    case STATE_GAMEOVER:
        DrawCenter(imgGameOver);
        break;

    case STATE_RESULT:
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, resultAlpha);
        DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        DrawStringToHandle(500, 200, resultText.c_str(), GetColor(255, 255, 0), fontStart);
        char finalScore[32];
        sprintf_s(finalScore, sizeof(finalScore), "FINAL SCORE: %d", resultScoreDisplay);
        DrawStringToHandle(540, 300, finalScore, GetColor(255, 255, 255), fontStart);

        for (auto& p : resultParticles)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, p.alpha);
            DrawCircle((int)p.x, (int)p.y, p.r, GetColor(255, 255, 255), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }

        if (DrawButton(480, 400, "タイトルへ", fontStart, GetColor(255, 255, 255), GetColor(255, 255, 0)))
        {
            state = STATE_TITLE;
            score = 0;
        }
        break;
    }

    // マウスカーソル描画
    int mx, my;
    GetMousePoint(&mx, &my);
    int color = GetColor(255, 220, 120);
    DrawCircle(mx, my, 3, color, TRUE);
    DrawCircle(mx, my, 10, color, FALSE);
    DrawLine(mx - 16, my, mx - 6, my, color);
    DrawLine(mx + 6, my, mx + 16, my, color);
    DrawLine(mx, my - 16, mx, my - 6, color);
    DrawLine(mx, my + 6, mx, my + 16, color);
}

// ---------------- オブジェクト描画（Yソート） ----------------
void Game::DrawGame()
{
    struct DrawObj { float y; std::function<void()> drawFunc; };
    std::vector<DrawObj> drawList;

    drawList.push_back({ player.y,[&]() {player.Draw();} });
    for (auto& e : EnemyManager::enemies) drawList.push_back({ e.y,[&e]() {e.Draw();} });
    for (auto& b : BulletManager::bullets) drawList.push_back({ b.y,[&b]() {b.Draw();} });

    std::sort(drawList.begin(), drawList.end(), [](const DrawObj& a, const DrawObj& b) {return a.y < b.y;});

    for (auto& obj : drawList) obj.drawFunc();
}