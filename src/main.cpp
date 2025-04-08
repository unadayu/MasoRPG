#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <iostream>

//

void drawText(SDL_Renderer* renderer, float R, float G, float B, TTF_Font* font, std::string Text, float x, float y)
{
    // テキストの色を指定
    SDL_Color color = {static_cast<Uint8>(R), static_cast<Uint8>(G), static_cast<Uint8>(B)};
    
    // テキストをサーフェスに変換
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, Text.c_str(), color);
    if (!surface) {
        std::cerr << "テキストのレンダリングに失敗しました: " << TTF_GetError() << std::endl;
        return;
    }

    // サーフェスからテクスチャを作成
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // サーフェスは不要になったので解放

    if (!texture) {
        std::cerr << "テクスチャの作成に失敗しました: " << SDL_GetError() << std::endl;
        return;
    }

    // テクスチャの描画位置とサイズを設定
    SDL_Rect dstRect = {static_cast<int>(x), static_cast<int>(y), surface->w, surface->h};

    // テクスチャをレンダラーに描画
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    // テクスチャを解放
    SDL_DestroyTexture(texture);
}

bool isKeyPressed(SDL_Event& event, SDL_Keycode key) {
    return (event.type == SDL_KEYDOWN && event.key.keysym.sym == key);
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // SDL_ttfの初期化
    if (TTF_Init() == -1) {
      std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
      SDL_Quit();
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
    Rectangle titleCursor = { 10, 10, 3, 250};

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

    bool title = true;

    // フォント読み込み（.ttfファイルが必要）
    TTF_Font* noJapaneseFontTitle = TTF_OpenFont("fonts/8-bit-no-ja/8bitOperatorPlus8-Bold.ttf", 50);
    TTF_Font* noJapaneseFont = TTF_OpenFont("fonts/8-bit-no-ja/8bitOperatorPlus8-Bold", 24);
    TTF_Font* japaneseFont = TTF_OpenFont("fonts/ja-16-bit/DotGothic16-Regular.ttf", 24);

    // メインループフラグとイベント構造体
    bool running = true;
    SDL_Event event;

    while (running) {
        // イベント処理
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            // ESCキーを押した時だけ終了
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }

                if (title)
                {
                  // ↓キーが押された瞬間だけ処理
                  if (isKeyPressed(event, SDLK_DOWN)) {
                    titleCursor.y += 30.0f; // カーソルを下に移動
                  }
                  // ↑キーが押された瞬間だけ処理
                  if (isKeyPressed(event, SDLK_UP)) {
                    titleCursor.y -= 30.0f; // カーソルを上に移動
                  }
                  
                  if (isKeyPressed(event, SDLK_RETURN)) if (titleCursor.y = 250) title = false;
                }
            }
        }

        if (title)
        {
          // 背景色設定＆クリア（青）
          SDL_SetRenderDrawColor(renderer, 0, 184, 255, 255);
          SDL_RenderClear(renderer);

          if (titleCursor.y < 250) titleCursor.y = 250;
          if (titleCursor.y > 310) titleCursor.y = 310;

          drawText(renderer, 0.0f, 0.0f, 0.0f, noJapaneseFontTitle, "MasoRPG", 10, 30.0f);
          drawText(renderer, 0.0f, 0.0f, 0.0f, japaneseFont, "スタート", 20, 250.0f);
          drawText(renderer, 0.0f, 0.0f, 0.0f, japaneseFont, "設定", 20, 280.0f);
          drawText(renderer, 0.0f, 0.0f, 0.0f, japaneseFont, "おわり", 20, 310.0f);
          drawText(renderer, 255.0f, 0.0f, 0.0f, japaneseFont, ">", titleCursor.x, titleCursor.y);
          // 描画反映
          SDL_RenderPresent(renderer);

          // 少し待つ（CPU負荷軽減）
          SDL_Delay(8); // 約60FPS
        }
        else {
          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
          SDL_RenderClear(renderer);

          SDL_RenderPresent(renderer);
          SDL_Delay(16);
        }
    }
    TTF_CloseFont(noJapaneseFontTitle);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

