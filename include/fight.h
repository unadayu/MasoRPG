// include/fight.h
#ifndef FIGHT_H
#define FIGHT_H

#include <iostream>
#include <SDL2/SDL.h>
#include <filesystem>
#include <vector>
#include <cmath>
#include <algorithm>
#include "function.h"

class fightUI {
public:
    // main.cpp からはこのシグネチャで呼び出す
    void lastboss(SDL_Renderer* renderer, Rectangle WindowSize, std::filesystem::path& fontPath, SDL_Texture* bossTexture, Camera2D camera, SDL_Event& event, SDL_Texture* playerTexture, enemy& enemyData)
    {
        this->fontPath = fontPath;
        update(event, WindowSize, enemyData, renderer);
        render(renderer, WindowSize, bossTexture, camera, playerTexture);
    }

private:
    struct Command { const char* label; };

    // function.h 側の定義を使う
    InGameTurnSettings playerData = {0,0,true};    // = {0,1,true} は main で初期化してもOK
    InGamePlayerData playerRect;    // = {初期x, 初期y, width, height}
    std::vector<Command> commands = { {"バトル"}, {"スキル"}, {"チャージ"}, {"逃げる"} };
    int selectedIndex = 0;
    std::filesystem::path fontPath;
    Uint32 turnStartTime = 0;
    Uint32 elapsed = 0;
    Uint32 now = 0;
    bool isTungFirstTurnActive = false;

    Bullet slashAttackRect = {800/2 - 110, 500 - 230, 3, 3, 8, 200, nullptr, 0, 3, 5};
    Bullet gyakuSlashAttackRect = {800/2 - 110 + 200, 500 - 230, 3, 3, 8, 200, nullptr, 0, 3, 5};

    void slashAttack(SDL_Renderer* renderer) {
        DrawRectangle(slashAttackRect.x, slashAttackRect.y, slashAttackRect.width, slashAttackRect.height, (SDL_Color){255, 0, 0, 0}, renderer);
        slashAttackRect.x += slashAttackRect.speed;
    }

    void gyakuSlashAttack(SDL_Renderer* renderer) {
        DrawRectangle(gyakuSlashAttackRect.x, gyakuSlashAttackRect.y, gyakuSlashAttackRect.width, gyakuSlashAttackRect.height, (SDL_Color){255, 0, 0, 0}, renderer);
        gyakuSlashAttackRect.x -= gyakuSlashAttackRect.speed;
    }

    void update(SDL_Event& event, Rectangle WindowSize, enemy& enemyData, SDL_Renderer* renderer) {
        if (playerData.playerTurn) {
            processPlayerInput();
        } else {
            processEnemyTurn(WindowSize, enemyData, renderer);
        }
    }

    void processPlayerInput() {
        if (isKeyTapped(SDLK_UP)) playerRect.y -= 90;
        if (isKeyTapped(SDLK_DOWN)) playerRect.y += 90;
        playerRect.x = 25;
        if (playerRect.y <= 83) playerRect.y = 83;
        if (playerRect.y >= 353) playerRect.y = 353;

        std::cout << playerData.turn << std::endl;

        if (playerRect.y >= 83){
            if (isKeyTapped(SDLK_RETURN)) {
                playerData.playerTurn = false;
                playerData.turn += 1;
            }
        }
    }

    void processEnemyTurn(Rectangle WindowSize, enemy& enemyData, SDL_Renderer* renderer) {
        // "tung" の最初のターンなら 3 秒間プレイヤーを中央四角内に制限
        if (enemyData.name == "tung" && playerData.turn == 1) {
            if (!isTungFirstTurnActive) {
                turnStartTime = SDL_GetTicks();
                isTungFirstTurnActive = true;
            }

            now = SDL_GetTicks();
            elapsed = now - turnStartTime;
            std::cout << elapsed << std::endl;
            if (elapsed < 3000) {
                // WindowSize.Width/2 - 110, WindowSize.Height - 230, 200, 200
                // 入力でプレイヤー移動
                if (isKeyTapped(SDLK_LEFT))  playerRect.x -= 5;
                if (isKeyTapped(SDLK_RIGHT)) playerRect.x += 5;
                if (isKeyTapped(SDLK_UP))    playerRect.y -= 5;
                if (isKeyTapped(SDLK_DOWN))  playerRect.y += 5;

                // 中央四角領域のサイズ（例として幅400×高さ400）
                const int W = 400, H = 400;
                int boxX = WindowSize.Width/2 - 110;
                int boxY = WindowSize.Height - 230;

                // 手動クランプ
                if (playerRect.x < boxX) playerRect.x = boxX;
                if (playerRect.x + 30 > boxX + W) playerRect.x = boxX + W - 30;
                if (playerRect.y < boxY) playerRect.y = boxY;
                if (playerRect.y + 30 > boxY + H) playerRect.y = boxY + H - 30;
                slashAttack(renderer);
                gyakuSlashAttack(renderer);
            } else {
                playerData.playerTurn = true;
                playerData.turn ++;
            }
        }
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
