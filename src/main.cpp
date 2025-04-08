#include <SDL2/SDL.h>
#include <iostream>

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // ウィンドウサイズ
    struct Rectangle {
        float Width;
        float Height;
        float x;
        float y;
    };

    Rectangle WindowSise = { 800, 500, 0, 0 };

    SDL_Window* window = SDL_CreateWindow(
        "SDL Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WindowSise.Width,
        WindowSise.Height,
        SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // メインループフラグとイベント構造体
    bool running = true;
    SDL_Event event;

    while (running) {
        // イベント処理
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            // ESCで強制終了
            if (event.type == SDL_KEYDOWN)
            {
              running = false;
            }
        }

        // 背景色設定＆クリア（青）
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderClear(renderer);

        // 描画反映
        SDL_RenderPresent(renderer);

        // 少し待つ（CPU負荷軽減）
        SDL_Delay(16); // 約60FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

