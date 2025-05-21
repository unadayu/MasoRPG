#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <filesystem>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>
#include <memory>
#include "function.h"

class fightUI {
public:
  void runGameGame(SDL_Renderer* renderer) {
    PlayerFlame(renderer);
  }

private:
  InGamePlayerData playerRect = {0, 0, 9315, 9315};

  void flame(SDL_Renderer* renderer) {
    // ステーたす
    DrawRectangleLines(30, 30, 100, 250, SDL_Color{ 255, 255, 255, 255}, renderer);
    
    // 背景
    DrawRectangleLines(260, 30, 450, 250, SDL_Color{ 255, 255, 255, 255}, renderer);
    
    // 偽証
    DrawRectangleLines(30, 310, 100, 250, SDL_Color{ 255, 255, 255, 255}, renderer);
    
    // コマンドの枠
    // バトル
    DrawRectangleLines(260, 320, 60, 30, SDL_Color{ 255, 255, 255, 255}, renderer);
    // スキル
    DrawRectangleLines(260, 390, 60, 30, SDL_Color{ 255, 255, 255, 255}, renderer);
    // チャージ
    DrawRectangleLines(260, 450, 60, 30, SDL_Color{ 255, 255, 255, 255}, renderer);
    // 逃げる
    DrawRectangleLines(260, 510, 60, 30, SDL_Color{ 255, 255, 255, 255}, renderer);
  }

  void background(SDL_Texture* back, SDL_Renderer* renderer, Rectangle WindowSize) {
    SDL_Rect backRect = { 0, 0, WindowSize.Width, WindowSize.Height };
    SDL_RenderCopy(renderer, back, nullptr, &backRect);
  }

  void playerMove(const SDL_Event& event) {
    if (isKeyTapped(event, SDLK_UP)) playerRect.y -= 30;
    if (isKeyTapped(event, SDLK_DOWN)) playerRect.y += 30;
    if (isKeyTapped(event, SDLK_LEFT)) playerRect.x -= 30;
    if (isKeyTapped(event, SDLK_RIGHT)) playerRect.y += 30;
  }

  void PlayerFlame(SDL_Renderer* renderer) {
    // TDN枠
    DrawRectangleLines(425, 310, 120, 120, SDL_Color{ 255, 255, 255, 255}, renderer);
    
    // 縦線
    DrawRectangleLines(455, 310, 120, 1, SDL_Color{ 255, 255, 255, 255}, renderer);
    DrawRectangleLines(505, 310, 120, 1, SDL_Color{ 255, 255, 255, 255}, renderer);

    // ━線
    DrawRectangleLines(425, 340, 1, 120, SDL_Color{ 255, 255, 255, 255}, renderer);
    DrawRectangleLines(425, 370, 1, 120, SDL_Color{ 255, 255, 255, 255}, renderer);
  }
};

// とりあいず昔書いたコード
// ありえんほど汚い
//
//                      if (phase = 1)
//                     {
//                         if (RPGCommandnumber == 1)
//                         {
//                             if (InGamePlayerRect.x <= 190) InGamePlayerRect.x = 190;
//                             if (InGamePlayerRect.x >= 390) InGamePlayerRect.x = 390;
//                             if (attackOne == 1) drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "殴る", 230, 200);
//                             else if (attackOne == 2) drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "蹴る", 230, 200);
//                             else if (attackOne == 3) drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "ちんこ", 230, 200);
//
//                             if (attackTwo == 1) drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "殴る", 330, 200);
//                             else if (attackTwo == 2) drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "蹴る", 330, 200);
//                             else if (attackTwo == 3) drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "ちんこ", 330, 200);
//
//                             if (attackThree == 1) drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "殴る", 430, 200);
//                             else if (attackThree == 2) drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "蹴る", 430, 200);
//                             else if (attackThree == 3) drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "ちんこ", 430, 200);
//                         }
//                         else
//                         {
//                             if (InGamePlayerRect.x <= 30) InGamePlayerRect.x = 30;
//                             if (InGamePlayerRect.x >= 600) InGamePlayerRect.x = 600;
//                         }
//                         //ここに敵表示
//                         SDL_Rect bossRect = { 300, 10, 200, 200 };
//                         SDL_RenderCopy(renderer, bossTexture, nullptr, &bossRect);
//                         DrawRectangleLines(200, 200, 380, 180, SDL_Color{ 255, 255, 255, 255}, renderer);
//
//                         // 白
//                         DrawRectangle(20, 400, 180, 60, SDL_Color{ 255, 255, 255, 255 }, renderer);
//                         DrawRectangle(210, 400, 180, 60, SDL_Color{ 255, 255, 255, 255 }, renderer);
//                         DrawRectangle(400, 400, 180, 60, SDL_Color{ 255, 255, 255, 255 }, renderer);
//                         DrawRectangle(590, 400, 180, 60, SDL_Color{ 255, 255, 255, 255 }, renderer);
//                         // 黒
//                         DrawRectangle(25, 405, 170, 50, SDL_Color{ 0, 0, 0, 0 }, renderer);
//                         DrawRectangle(215, 405, 170, 50, SDL_Color{ 0, 0, 0, 0 }, renderer);
//                         DrawRectangle(405, 405, 170, 50, SDL_Color{ 0, 0, 0, 0 }, renderer);
//                         DrawRectangle(595, 405, 170, 50, SDL_Color{ 0, 0, 0, 0 }, renderer);
//
//                         std::cout << InGamePlayerRect.x << std::endl;
//                         drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "しね", 90, 410);
//                         drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "煽る", 280, 410);
//                         drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "食事", 470, 410);
//                         drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "逃げる", 660, 410);
//
//                         // ここにプレいやー
//                         SDL_Rect InGamePlayerRectSDL = {InGamePlayerRect.x, InGamePlayerRect.y, InGamePlayerRect.Width, InGamePlayerRect.Height};
//                         SDL_RenderCopy(renderer, woodLightTexture, nullptr, &InGamePlayerRectSDL);
//                     }
//
