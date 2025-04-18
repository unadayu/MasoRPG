#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <SDL_image.h>
#include "Camera2D.h"
#include <SDL_mixer.h>
#include <filesystem>

// プレイヤーやオブジェクトの矩形
struct Rectangle {
    int x;
    int y;
    int Width;
    int Height;
};

// テキスト描画関数
void drawText(SDL_Renderer* renderer, float R, float G, float B, TTF_Font* font, std::string Text, float x, float y)
{
    SDL_Color color = {static_cast<Uint8>(R), static_cast<Uint8>(G), static_cast<Uint8>(B)};
    
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, Text.c_str(), color);
    if (!surface) {
        std::cerr << "テキストのレンダリングに失敗しました: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // サーフェス解放

    if (!texture) {
        std::cerr << "テクスチャの作成に失敗しました: " << SDL_GetError() << std::endl;
        return;
    }

    int w = surface->w;
    int h = surface->h;
    
    SDL_Rect dstRect = {static_cast<int>(x), static_cast<int>(y), w, h};
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    SDL_DestroyTexture(texture);
}

// キーが押されたかどうかをチェックする関数
bool isKeyPressed(SDL_Event& event, SDL_Keycode key) {
    return (event.type == SDL_KEYDOWN && event.key.keysym.sym == key);
}

int main() {
    std::filesystem::path basePath = std::filesystem::current_path();
    std::filesystem::path ikisugiMusicPath = basePath / "compiler" / "run" / "data" / "music" / "ikisugiyou.wav";
    std::filesystem::path lethal_chinpoMusicPath = basePath / "compiler" / "run" / "data" / "music" / "lethalchinpo.wav";
    
    std::filesystem::path noJapaneseFontFontsPath = basePath / "compiler"  / "run" / "data" / "fonts" / "8-bit-no-ja" / "8bitOperatorPlus8-Bold.ttf";
    std::filesystem::path dotGothicFontsPath = basePath / "compiler"  / "run" / "data" / "fonts" / "ja-16-bit" / "DotGothic16-Regular.ttf";

    std::filesystem::path woodLightImagePath = basePath / "compiler"  / "run" / "data" / "image" / "woodLight.png";


    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() == -1) {
      std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
      SDL_Quit();
      return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    // 音楽ファイルの読み込み
    Mix_Music* ikisugi = Mix_LoadMUS(ikisugiMusicPath.string().c_str());
    Mix_Music* lethal_chinpo = Mix_LoadMUS(lethal_chinpoMusicPath.string().c_str());

    Rectangle WindowSise = { 0, 0, 800, 500 };
    Rectangle titleCursor = { 3, 250, 10, 10};

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

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool title = true;
    int roomNumber = 114514;

    Rectangle player = {5000, 5000, 50, 50};
    SDL_Rect playerRect = { player.x, player.y, player.Width, player.Height };

    TTF_Font* noJapaneseFontTitle = TTF_OpenFont(noJapaneseFontFontsPath.string().c_str(), 50);
    TTF_Font* noJapaneseFont = TTF_OpenFont(noJapaneseFontFontsPath.string().c_str(), 24);
    TTF_Font* japaneseFont = TTF_OpenFont(dotGothicFontsPath.string().c_str(), 24);

    SDL_Surface* woodLightImage = IMG_Load(woodLightImagePath.string().c_str());

    SDL_Texture* woodLightTexture = SDL_CreateTextureFromSurface(renderer, woodLightImage);
    SDL_FreeSurface(woodLightImage);

    Camera2D camera(WindowSise.Width, WindowSise.Height, 10000, 10000);

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }

                if (title) {
                    if (isKeyPressed(event, SDLK_DOWN)) {
                        titleCursor.y += 30.0f;
                    }
                    if (isKeyPressed(event, SDLK_UP)) {
                        titleCursor.y -= 30.0f;
                    }
                    
                    if (isKeyPressed(event, SDLK_RETURN) && titleCursor.y == 250) {
                        title = false;
                    }
                }
            }
        }

        if (title) {
            if (!Mix_PlayingMusic()) {
                Mix_PlayMusic(lethal_chinpo, -1);
            }
            SDL_SetRenderDrawColor(renderer, 0, 184, 255, 255);
            SDL_RenderClear(renderer);

            if (titleCursor.y < 250) titleCursor.y = 250;
            if (titleCursor.y > 310) titleCursor.y = 310;

            drawText(renderer, 0.0f, 0.0f, 0.0f, noJapaneseFontTitle, "MasoRPG", 10, 30.0f);
            drawText(renderer, 0.0f, 0.0f, 0.0f, japaneseFont, "スタート", 20, 250.0f);
            drawText(renderer, 0.0f, 0.0f, 0.0f, japaneseFont, "設定", 20, 280.0f);
            drawText(renderer, 0.0f, 0.0f, 0.0f, japaneseFont, "おわり", 20, 310.0f);
            drawText(renderer, 255.0f, 0.0f, 0.0f, japaneseFont, ">", titleCursor.x, titleCursor.y);

            SDL_RenderPresent(renderer);
            SDL_Delay(8);
        } else {
            if (Mix_PlayingMusic()) {
                Mix_HaltMusic();
            }
            camera.follow(playerRect);
            camera.setPosition(playerRect.x, playerRect.y);
            camera.clampPosition(10000, 10000);

            SDL_RenderClear(renderer);

            SDL_Rect screenRect = camera.worldToScreen(playerRect);
            SDL_RenderCopy(renderer, woodLightTexture, nullptr, &screenRect);

            if (isKeyPressed(event, SDLK_UP)) playerRect.y -= 5;
            if (isKeyPressed(event, SDLK_DOWN)) playerRect.y += 5;
            if (isKeyPressed(event, SDLK_LEFT)) playerRect.x -= 5;
            if (isKeyPressed(event, SDLK_RIGHT)) playerRect.x += 5;

            std::cout << playerRect.x << "\n"
            << playerRect.y << std::endl;
            if (playerRect.x <= -15) playerRect.x = -15;
            if (playerRect.y <= -10) playerRect.y = -10;
            if (playerRect.x >= 755) playerRect.x = 755;
            if (playerRect.y >= 450) playerRect.y = 450;

            SDL_RenderCopy(renderer, woodLightTexture, nullptr, &screenRect);
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }
    }

    TTF_CloseFont(noJapaneseFontTitle);
    TTF_CloseFont(noJapaneseFont);
    TTF_CloseFont(japaneseFont);
    SDL_DestroyTexture(woodLightTexture);
    Mix_FreeMusic(ikisugi);
    Mix_FreeMusic(lethal_chinpo);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
