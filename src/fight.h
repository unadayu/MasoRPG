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

namespace fightUI {
  void flame(SDL_Renderer* renderer) {
    // ステーたす
    DrawRectangleLines(30, 30, 100, 250, SDL_Color{ 255, 255, 255, 255}, renderer);
    
    // 背景
    DrawRectangleLines(260, 30, 450, 250, SDL_Color{ 255, 255, 255, 255}, renderer);
    
    // 偽証
    DrawRectangleLines(30, 310, 100, 250, SDL_Color{ 255, 255, 255, 255}, renderer);
    
    // コマンドの枠
    // しね
    DrawRectangleLines(260, 320, 60, 30, SDL_Color{ 255, 255, 255, 255}, renderer);
    // コ
    DrawRectangleLines(260, 390, 60, 30, SDL_Color{ 255, 255, 255, 255}, renderer);
    // チャ
    DrawRectangleLines(260, 450, 60, 30, SDL_Color{ 255, 255, 255, 255}, renderer);
    // に
    DrawRectangleLines(260, 510, 60, 30, SDL_Color{ 255, 255, 255, 255}, renderer);
  }
}
