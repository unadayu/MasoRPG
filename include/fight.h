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
  void runChinpo() {
    PlayerFlame();
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

  void background(std::filesystem::path back, SDL_Renderer* renderer, Rectangle WindowSize) {
    SDL_Surface* backImage = IMG_Load(back.string().c_str());
    SDL_Texture* backTexture = SDL_CreateTextureFromSurface(renderer, backImage);
    SDL_FreeSurface(backImage);
    SDL_Rect backRect = { 0, 0, WindowSize.Width, WindowSize.Height };
    SDL_RenderCopy(renderer, backTexture, nullptr, &backRect);
  }

  void playerMove(const SDL_Event& event) {
    if (isKeyTapped(event, SDLK_UP)) playerRect.y -= 30;
    if (isKeyTapped(event, SDLK_DOWN)) playerRect.y += 30;
    if (isKeyTapped(event, SDLK_LEFT)) playerRect.x -= 30;
    if (isKeyTapped(event, SDLK_RIGHT)) playerRect.y += 30;
  }
  void PlayerFlame() {
    // TDN枠
    DrawRectangleLines(425, 310, 120, 120, SDL_Color{ 255, 255, 255, 255}, renderer);
    
    // 縦線
    DrawRectangleLines(455, 310, 120, 1, SDL_Color{ 255, 255, 255, 255}, renderer);
    DrawRectangleLines(505, 310, 120, 1, SDL_Color{ 255, 255, 255, 255}, renderer);

    // ━線
    DrawRectangleLines(425, 340, 1, 120, SDL_Color{ 255, 255, 255, 255}, renderer);
    DrawRectangleLines(425, 370, 1, 120, SDL_Color{ 255, 255, 255, 255}, renderer);
  }
}
