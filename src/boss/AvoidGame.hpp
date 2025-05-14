// AvoidGame.hpp
#ifndef AVOID_GAME_HPP
#define AVOID_GAME_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include "function.h"

struct Obstacle {
    SDL_Rect rect;
    int       speed;
};

struct WarningLine {
    int      x1, y1, x2, y2;
    SDL_Rect bbox;
    bool     isHorizontal;
    Uint32   startTime;
    bool     solid;
};

class AvoidGame {
public:
    static constexpr int LINE_THICKNESS = 3;

    AvoidGame(int w, int h, int hp)
      : width(w), height(h),
        window(nullptr), renderer(nullptr),
        playerTex(nullptr), sceneTex(nullptr),
        music(nullptr),
        health(hp), maxHealth(hp),
        displayedHealthWidth(0.0f),
        lastDamage(0), lastObs(0),
        lastLine(0), lastGlitch(0),
        glitchActive(false)
    {
        std::srand((unsigned)std::time(nullptr));
    }

    ~AvoidGame() {
        // SDL cleanup
        if (music)      Mix_FreeMusic(music);
        Mix_CloseAudio();
        IMG_Quit();
        if (playerTex)  SDL_DestroyTexture(playerTex);
        if (sceneTex)   SDL_DestroyTexture(sceneTex);
        if (renderer)   SDL_DestroyRenderer(renderer);
        if (window)     SDL_DestroyWindow(window);
        SDL_Quit();
    }

    // title: ウィンドウタイトル
    // playerImg: プレイヤー画像パス（nullptr可）
    // bgmFile:   BGMファイルパス（nullptr可）
    bool init(const char* title,
              const char* playerImg = nullptr,
              const char* bgmFile   = nullptr)
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return false;
        window   = SDL_CreateWindow(title,
                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                    width, height, 0);
        if (!window) return false;
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) return false;

        IMG_Init(IMG_INIT_PNG);
        if (playerImg) {
            SDL_Surface* surf = IMG_Load(playerImg);
            if (surf) {
                playerTex = SDL_CreateTextureFromSurface(renderer, surf);
                int pw = surf->w, ph = surf->h;
                SDL_FreeSurface(surf);
                // 最大 50×50 に縮小
                const int maxW = 50, maxH = 50;
                float scale = std::min((float)maxW/pw, (float)maxH/ph);
                int newW = int(pw*scale), newH = int(ph*scale);
                playerRect = {
                    width/2 - newW/2,
                    height - newH - 10,
                    newW, newH
                };
            }
        } else {
            playerRect = { width/2 - 25, height - 60, 50, 50 };
        }

        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
        if (bgmFile) {
            music = Mix_LoadMUS(bgmFile);
            if (music) Mix_PlayMusic(music, -1);
        }

        // シーン用オフスクリーン
        sceneTex = SDL_CreateTexture(renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            width, height);

        lastObs    = SDL_GetTicks();
        lastLine   = SDL_GetTicks() + 3000; // 3秒後
        lastGlitch = SDL_GetTicks() + 2000; // 2秒後
        return true;
    }

    void run() {
        bool running = true;
        gameStartTime = SDL_GetTicks();
        while (running) {
            Uint32 now = SDL_GetTicks();

            std::cout << now - gameStartTime << "         " << healthRebornMAX << std::endl;
            // イベント＆入力
            SDL_Event e;
            while (SDL_PollEvent(&e))
                if (e.type == SDL_QUIT) running = false;

            const Uint8* ks = SDL_GetKeyboardState(nullptr);
            if (ks[SDL_SCANCODE_LEFT]  && playerRect.x > 0)
                playerRect.x -= 5;
            if (ks[SDL_SCANCODE_RIGHT] &&
                playerRect.x + playerRect.w < width)
                playerRect.x += 5;
            if (ks[SDL_SCANCODE_UP]    && playerRect.y > 0)
                playerRect.y -= 5;
            if (ks[SDL_SCANCODE_DOWN]  &&
                playerRect.y + playerRect.h < height)
                playerRect.y += 5;

            if (isKeyTapped(e, SDLK_c)) healthReborn(1);
            now = SDL_GetTicks();
            if (now - gameStartTime > 16800 && now - gameStartTime < 200500) {
                // スポーン制御
                if (now - lastObs > 50) {
                    spawnObstacle();
                    lastObs = now;
                }
                if (now >= lastLine) {
                    spawnWarningLine();
                    lastLine = now + (std::rand()%2000);
                }
                if (!glitchActive && now >= lastGlitch) {
                    glitchActive = true;
                    lastGlitch   = now + 500;
                } else if (glitchActive && now >= lastGlitch) {
                    glitchActive = false;
                    lastGlitch   = now + (std::rand()%2000 + 2000);
                }
            }
            else if (now - gameStartTime > 200500)
            {
                Mix_HaltMusic();
                spawnWarningLine();
            }

            // 更新：弾幕
            for (auto& o : obstacles) o.rect.y += o.speed;
            obstacles.erase(
              std::remove_if(obstacles.begin(), obstacles.end(),
                [&](auto& o){ return o.rect.y > height; }),
              obstacles.end());

            // 更新：警告線の状態＆削除
            for (auto& L : lines) {
                if (!L.solid && now - L.startTime > 1000)
                    L.solid = true;
            }
            lines.erase(
              std::remove_if(lines.begin(), lines.end(),
                [&](const WarningLine& L){
                    return now - L.startTime > 2000;
                }),
              lines.end());

            // 当たり判定（ヒットボックス内側縮小）
            SDL_Rect hb = {
                playerRect.x + 10, playerRect.y + 10,
                playerRect.w - 20, playerRect.h - 20
            };
            for (auto& o : obstacles) {
                if (SDL_HasIntersection(&hb, &o.rect)) {
                    takeDamage(5);
                }
            }
            for (auto& L : lines) {
                if (!L.solid) continue;
                if (L.isHorizontal) {
                    if (SDL_HasIntersection(&hb, &L.bbox))
                        takeDamage(health);
                } else {
                    int cx = hb.x + hb.w/2, cy = hb.y + hb.h/2;
                    float num = std::abs((L.y2-L.y1)*cx
                                      - (L.x2-L.x1)*cy
                                      + L.x2*L.y1
                                      - L.y2*L.x1);
                    float den = std::sqrt(float((L.y2-L.y1)*(L.y2-L.y1)
                                              + (L.x2-L.x1)*(L.x2-L.x1)));
                    if (den > 0 && num/den < LINE_THICKNESS)
                        takeDamage(health);
                }
            }

            // 描画
            draw(e);
            SDL_Delay(16);
        }
    }

private:
    int width, height;
    SDL_Window*   window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture*  playerTex = nullptr;
    SDL_Rect      playerRect;
    SDL_Texture*  sceneTex = nullptr;
    Mix_Music*    music = nullptr;

    std::vector<Obstacle>    obstacles;
    std::vector<WarningLine> lines;

    int health, maxHealth;
    float  displayedHealthWidth;
    Uint32 lastDamage, lastObs, lastLine, lastGlitch;
    bool glitchActive;
    Uint32 gameStartTime = 0;
    Uint32 now;

    int healthRebornMAX = 2;

    void spawnObstacle() {
        int w  = 10 + std::rand()%60;
        int x  = std::rand()%(width - w);
        int h  = 10 + std::rand()%20;
        int sp = 2 + std::rand()%9;
        obstacles.push_back({ SDL_Rect{x,0,w,h}, sp });
    }

    void spawnWarningLine() {
        Uint32 now = SDL_GetTicks();
        if (std::rand()%2 == 0) {
            int y = std::rand()%height;
            lines.push_back({
                0, y, width, y,
                SDL_Rect{0,y,width,LINE_THICKNESS},
                true, now, false
            });
        } else {
            bool mainDiag = std::rand()%2==0;
            int x1 = mainDiag?0:width, y1=0;
            int x2 = mainDiag?width:0, y2=height;
            lines.push_back({
                x1,y1,x2,y2,
                SDL_Rect{
                  std::min(x1,x2),
                  std::min(y1,y2),
                  std::abs(x2-x1),
                  std::abs(y2-y1)
                },
                false, now, false
            });
        }
    }

    void takeDamage(int d) {
        if (health <= 0) return;
        health = std::max(0, health - d);
        lastDamage = SDL_GetTicks();  // ダメージ時刻を更新
        if (health <= 0) {
            SDL_Quit();
            std::exit(0);
        }
    }

    void healthReborn(int remove) {
        healthRebornMAX -= remove;
        health = 2000;
    }

    void draw(SDL_Event& event) {
        Uint32 now = SDL_GetTicks();

        // オフスクリーン描画
        SDL_SetRenderTarget(renderer, sceneTex);
        SDL_SetRenderDrawColor(renderer, 30,30,30,255);
        SDL_RenderClear(renderer);

        // 弾幕
        SDL_SetRenderDrawColor(renderer, 200,0,0,255);
        for (auto& o : obstacles)
            SDL_RenderFillRect(renderer, &o.rect);

        // 警告線
        for (auto& L : lines) {
            Uint32 dt = now - L.startTime;
            if (!L.solid && dt < 1000) {
                SDL_SetRenderDrawColor(renderer, 255,255,0,255);
                SDL_RenderDrawLine(renderer, L.x1,L.y1, L.x2,L.y2);
            }
            else if (L.solid && dt < 2000) {
                SDL_SetRenderDrawColor(renderer, 255,0,0,255);
                int half = LINE_THICKNESS/2;
                for (int t=-half; t<=half; ++t) {
                    SDL_RenderDrawLine(renderer,
                        L.x1 + t, L.y1 + t,
                        L.x2 + t, L.y2 + t);
                }
            }
        }

        // プレイヤー
        if (playerTex)
            SDL_RenderCopy(renderer, playerTex, nullptr, &playerRect);
        else {
            SDL_SetRenderDrawColor(renderer, 0,200,0,255);
            SDL_RenderFillRect(renderer, &playerRect);
        }

        // 体力バー（ダメージ後1秒間だけ）
        if (now - lastDamage < 1000) {
            const int bw=60, bh=8;
            float targetW = health * float(bw) / maxHealth;
            displayedHealthWidth += (targetW - displayedHealthWidth) * 0.1f;
            int x = playerRect.x + (playerRect.w - bw)/2;
            int y = playerRect.y - bh - 6;
            SDL_Rect bg = { x, y, bw, bh };
            SDL_SetRenderDrawColor(renderer, 80,80,80,255);
            SDL_RenderFillRect(renderer, &bg);
            SDL_Rect fg = { x, y, int(displayedHealthWidth), bh };
            SDL_SetRenderDrawColor(renderer, 0,200,0,255);
            SDL_RenderFillRect(renderer, &fg);
        }

        SDL_SetRenderTarget(renderer, nullptr);
        SDL_RenderCopy(renderer, sceneTex, nullptr, nullptr);

        // グリッチ最前面
        if (glitchActive) {
            for (int i = 0; i < 1500; ++i) {
                SDL_SetRenderDrawColor(renderer,
                    std::rand()%150,
                    std::rand()%150,
                    std::rand()%150,
                    150);
                SDL_Rect n = {
                    std::rand()%width,
                    std::rand()%height,
                    1 + std::rand()%50,
                    1 + std::rand()%50
                };
                SDL_RenderFillRect(renderer, &n);
            }
        }

        SDL_RenderPresent(renderer);
    }
};

#endif // AVOID_GAME_HPP
