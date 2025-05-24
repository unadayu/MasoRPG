// include/fight.h
#ifndef FIGHT_H
#define FIGHT_H

#include <SDL2/SDL.h>
#include <filesystem>
#include <vector>
#include "function.h"   // ここに enemy, InGameTurnSettings, InGamePlayerData, isKeyTapped, drawImage などが定義されている前提

class fightUI {
public:
    // main.cpp からはこのシグネチャで呼び出す
    void lastboss(SDL_Renderer* renderer, Rectangle WindowSize, std::filesystem::path& fontPath, SDL_Texture* bossTexture, Camera2D camera, SDL_Event& event, SDL_Texture* playerTexture, enemy& enemyData)
    {
        this->fontPath = fontPath;
        update(event, WindowSize, enemyData);
        render(renderer, WindowSize, bossTexture, camera, playerTexture);
    }

private:
    struct Command { const char* label; };

    // function.h 側の定義を使う
    InGameTurnSettings         playerData;    // = {0,1,true} は main で初期化してもOK
    InGamePlayerData           playerRect;    // = {初期x, 初期y, width, height}
    std::vector<Command>       commands = { {"バトル"}, {"スキル"}, {"チャージ"}, {"逃げる"} };
    int                        selectedIndex = 0;
    std::filesystem::path      fontPath;
    Uint32                     turnStartTime = 0;

    void update(SDL_Event& event, Rectangle WindowSize, enemy& enemyData) {
        if (playerData.playerTurn) {
            processPlayerInput(event);
        } else {
            processEnemyTurn(event, WindowSize, enemyData);
        }
    }

    void processPlayerInput(SDL_Event& event) {
        playerRect.x = 25;
        if (playerRect.y <= 83) playerRect.y = 83;
        // if (isKeyTapped(event, SDLK_UP)) playerRect.y -= 90;
        if (isKeyTapped(SDLK_DOWN)) playerRect.y += 90;
        // if (isKeyTapped(event, SDLK_ESCAPE)) playerRect.y = 
    }

    void processEnemyTurn(SDL_Event& event, Rectangle WindowSize, enemy& enemyData) {
        // "tung" の最初のターンなら 3 秒間プレイヤーを中央四角内に制限
        if (enemyData.name == "tung" && playerData.turn == 1) {
            Uint32 now     = SDL_GetTicks();
            Uint32 elapsed = now - turnStartTime;
            if (elapsed < 3000) {
                // 入力でプレイヤー移動
                // if (isKeyTapped(event, SDLK_LEFT))  playerRect.x -= 5;
                // if (isKeyTapped(event, SDLK_RIGHT)) playerRect.x += 5;
                // if (isKeyTapped(event, SDLK_UP))    playerRect.y -= 5;
                // if (isKeyTapped(event, SDLK_DOWN))  playerRect.y += 5;

                // // 中央四角領域のサイズ（例として幅400×高さ400）
                // const int W = 400, H = 400;
                // int boxX = WindowSize.Width/2 - W/2;
                // int boxY = WindowSize.Height/2 - H/2;

                // // 手動クランプ
                // if (playerRect.x < boxX) playerRect.x = boxX;
                // if (playerRect.x + 30 > boxX + W) playerRect.x = boxX + W - 30;
                // if (playerRect.y < boxY) playerRect.y = boxY;
                // if (playerRect.y + 30 > boxY + H) playerRect.y = boxY + H - 30;
                return;
            }
        }
        // 3秒経過後 or その他の敵ターン
        playerData.playerTurn = true;
        playerData.turn += 1;
    }

    void render(SDL_Renderer* renderer,
                Rectangle WindowSize,
                SDL_Texture* bossTexture,
                Camera2D camera,
                SDL_Texture* playerTexture)
    {
        SDL_Color borderColor = {255,255,255,255};

        // ボス描画
        drawImage(WindowSize.Width/2 - 100, WindowSize.Height/2 - 200,
                  200, 200, bossTexture, renderer, camera);

        if (playerData.playerTurn) {
            // メニュー枠
            DrawRectangleLines(WindowSize.Width/2 - 200, WindowSize.Height - 210,
                               WindowSize.Width - 300, 200, borderColor, renderer);
            drawCommandMenu(renderer, WindowSize);

            // カーソル
            drawImage(playerRect.x, playerRect.y, 50, 50, playerTexture, renderer, camera);
        } else {
            // 敵ターン枠
            DrawRectangleLines(WindowSize.Width/2 - 110, WindowSize.Height - 230, 200, 200, borderColor, renderer);
        }
    }

    void drawCommandMenu(SDL_Renderer* renderer, Rectangle WindowSize) {
        const int boxX = 30, boxW = 150, boxH = 60, fontSize = 24;
        SDL_Color borderColor = {255,255,255,255};
        for (size_t i = 0; i < commands.size(); ++i) {
            int y = 80 + static_cast<int>(i)*90;
            DrawRectangleLines(boxX, y, boxW, boxH, borderColor, renderer);
            drawText(renderer, 255,255,255, fontPath, fontSize,
                     commands[i].label,
                     boxX+40, y+15);
        }
    }
};

#endif // FIGHT_H
