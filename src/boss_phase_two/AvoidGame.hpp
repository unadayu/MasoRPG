#ifndef AVOID_GAME_HPP
#define AVOID_GAME_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

struct Player { SDL_Rect rect; int speed; };
struct Obstacle { SDL_Rect rect; int speed; };
struct WarningLine { SDL_Rect rect; Uint32 startTime; bool dealt; };

class AvoidGame {
public:
    // w: width, h: height, hp: max health
    // Guard gauge max is fixed to 300
    AvoidGame(int w, int h, int hp = 50)
        : width(w), height(h), window(nullptr), renderer(nullptr),
          playerTexture(nullptr), sceneTexture(nullptr), running(false),
          health(hp), maxHealth(hp), lastObstacleTime(0),
          showHealthBar(false), lastDamageTime(0), healthBarDuration(2000),
          healthBarWidth(0), displayedHealthWidth(0),
          guardPoints(300), maxGuardPoints(300), lastGuardConsumeTime(0), guardConsumeInterval(200),
          guardBarWidth(0), displayedGuardWidth(0),
          glitchActive(false), lastGlitchTime(0),
          effectActive(false), nextEffectTime(0), effectStartTime(0), effectDuration(1000), effectType(0),
          nextWarningTime(0), isGuarding(false)
    {
        std::srand((unsigned)std::time(nullptr));
    }

    ~AvoidGame() {
        if (playerTexture) SDL_DestroyTexture(playerTexture);
        if (sceneTexture) SDL_DestroyTexture(sceneTexture);
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        IMG_Quit(); SDL_Quit();
    }

    bool init(const char* title, const char* playerImgPath = nullptr) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return false;
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  width, height, 0);
        if (!window) return false;
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) return false;
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        sceneTexture = SDL_CreateTexture(renderer,
            SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
            width, height);
        SDL_SetTextureBlendMode(sceneTexture, SDL_BLENDMODE_BLEND);

        player.rect = { width/2 - 25, height - 60, 50, 50 };
        player.speed = 8;
        healthBarWidth = displayedHealthWidth = player.rect.w;
        guardBarWidth = displayedGuardWidth = player.rect.w;
        guardPoints = maxGuardPoints = 30;

        if (playerImgPath) loadPlayerTexture(playerImgPath);

        Uint32 now = SDL_GetTicks();
        lastObstacleTime = lastGlitchTime = now;
        nextEffectTime = now + 2000 + rand() % 3000;
        nextWarningTime = now + 3000 + rand() % 3000;
        lastGuardConsumeTime = now;
        return true;
    }

    bool loadPlayerTexture(const char* filePath) {
        SDL_Surface* surf = IMG_Load(filePath);
        if (!surf) return false;
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_FreeSurface(surf);
        if (!tex) return false;
        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
        int defW = player.rect.w, defH = player.rect.h;
        int tw, th;
        SDL_QueryTexture(tex, NULL, NULL, &tw, &th);
        float r = std::min((float)defW/tw, (float)defH/th);
        if (r > 1) r = 1;
        player.rect.w = (int)(tw * r);
        player.rect.h = (int)(th * r);
        player.rect.x = (width - player.rect.w) / 2;
        player.rect.y = height - player.rect.h - 10;
        if (playerTexture) SDL_DestroyTexture(playerTexture);
        playerTexture = tex;
        healthBarWidth = displayedHealthWidth = player.rect.w;
        guardBarWidth = displayedGuardWidth = player.rect.w;
        return true;
    }

    void run() {
        running = true;
        while (running) {
            Uint32 now = SDL_GetTicks();
            handleEvents();
            update(now);
            draw(now);
            SDL_Delay(16);
        }
    }

private:
    int width, height;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Player player;
    SDL_Texture* playerTexture;
    SDL_Texture* sceneTexture;
    std::vector<Obstacle> obstacles;
    std::vector<WarningLine> warnings;
    bool running;

    int health, maxHealth;
    Uint32 lastObstacleTime;

    bool showHealthBar;
    Uint32 lastDamageTime;
    Uint32 healthBarDuration;
    int healthBarWidth, displayedHealthWidth;

    int guardPoints, maxGuardPoints;
    Uint32 lastGuardConsumeTime, guardConsumeInterval;
    int guardBarWidth, displayedGuardWidth;
    bool isGuarding;

    bool glitchActive;
    Uint32 lastGlitchTime;

    bool effectActive;
    Uint32 nextEffectTime, effectStartTime, effectDuration;
    int effectType;

    Uint32 nextWarningTime;

    void handleEvents() {
        SDL_Event e;
        while (SDL_PollEvent(&e)) if (e.type == SDL_QUIT) running = false;
        const Uint8* ks = SDL_GetKeyboardState(NULL);
        isGuarding = ks[SDL_SCANCODE_C] && guardPoints > 0;
        if (ks[SDL_SCANCODE_LEFT] && player.rect.x > 0) player.rect.x -= player.speed;
        if (ks[SDL_SCANCODE_RIGHT] && player.rect.x + player.rect.w < width) player.rect.x += player.speed;
        if (ks[SDL_SCANCODE_UP] && player.rect.y > 0) player.rect.y -= player.speed;
        if (ks[SDL_SCANCODE_DOWN] && player.rect.y + player.rect.h < height) player.rect.y += player.speed;
    }

    void update(Uint32 now) {
        // Spawn obstacles
        if (now - lastObstacleTime > (300 + rand() % 500)) {
            for (int i = 0; i < 2 + rand() % 3; ++i) addObstacle();
            lastObstacleTime = now;
        }
        // Move & remove obstacles
        for (auto& o : obstacles) o.rect.y += o.speed;
        obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(),
            [&](const Obstacle& o) { return o.rect.y > height; }), obstacles.end());
        // Reduced hitbox
        SDL_Rect hitbox = { player.rect.x + 10, player.rect.y + 10,
            player.rect.w - 20, player.rect.h - 20 };
        // Guard consumption and regen
        if (isGuarding && guardPoints > 0 && now - lastGuardConsumeTime >= guardConsumeInterval) {
            guardPoints = std::max(0, guardPoints - 3);
            lastGuardConsumeTime = now;
            if (guardPoints <= 0) isGuarding = false;
        } else if (!isGuarding && now - lastGuardConsumeTime >= guardConsumeInterval) {
            guardPoints = std::min(maxGuardPoints, guardPoints + 5);
            lastGuardConsumeTime = now;
        }
        // Collisions
        if (!isGuarding) {
            for (auto& o : obstacles) {
                if (SDL_HasIntersection(&hitbox, &o.rect)) {
                    if (--health <= 0) running = false;
                    showHealthBar = true;
                    lastDamageTime = now;
                    o.rect.y = height + 1;
                }
            }
        }
        handleGlitch(now);
        handleEffects(now);
        handleWarnings(now, hitbox);
        animateBars();
    }

    void handleGlitch(Uint32 now) {
        if (!glitchActive && now >= lastGlitchTime) {
            glitchActive = true;
            lastGlitchTime = now + 150;
        } else if (glitchActive && now >= lastGlitchTime) {
            glitchActive = false;
            lastGlitchTime = now + 1500;
        }
    }

    void handleEffects(Uint32 now) {
        if (!effectActive && now >= nextEffectTime) {
            effectActive = true;
            effectStartTime = now;
            effectType = rand() % 2;
            nextEffectTime = now + effectDuration + 2000 + rand() % 3000;
        } else if (effectActive && now >= effectStartTime + effectDuration) {
            effectActive = false;
        }
    }

    void handleWarnings(Uint32 now, const SDL_Rect& hitbox) {
        if (now >= nextWarningTime) {
            WarningLine w;
            int y = player.rect.y + (rand() % 2 ? -20 : player.rect.h + 20);
            w.rect = { player.rect.x, y, player.rect.w, 2 };
            w.startTime = now;
            w.dealt = false;
            warnings.push_back(w);
            nextWarningTime = now + 3000 + rand() % 3000;
        }
        for (auto& w : warnings) {
            if (!w.dealt && now >= w.startTime + 1000) {
                if (!isGuarding && SDL_HasIntersection(&hitbox, &w.rect)) {
                    if (--health <= 0) running = false;
                    showHealthBar = true;
                    lastDamageTime = now;
                }
                w.dealt = true;
            }
        }
        warnings.erase(std::remove_if(warnings.begin(), warnings.end(),
            [&](const WarningLine& w) { return now >= w.startTime + 2000; }), warnings.end());
    }

    void animateBars() {
        int targetHealthW = healthBarWidth * health / maxHealth;
        int diffH = displayedHealthWidth - targetHealthW;
        if (diffH > 0) displayedHealthWidth -= std::min(diffH, 6);
        else if (diffH < 0) displayedHealthWidth += std::min(-diffH, 6);
        int targetGuardW = guardBarWidth * guardPoints / maxGuardPoints;
        int diffG = displayedGuardWidth - targetGuardW;
        if (diffG > 0) displayedGuardWidth -= std::min(diffG, 6);
        else if (diffG < 0) displayedGuardWidth += std::min(-diffG, 6);
    }

    void drawSceneContent() {
        if (playerTexture) {
            if (isGuarding) SDL_SetTextureColorMod(playerTexture, 200, 200, 255);
            SDL_RenderCopy(renderer, playerTexture, NULL, &player.rect);
            if (isGuarding) SDL_SetTextureColorMod(playerTexture, 255, 255, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, isGuarding ? 0 : 0, isGuarding ? 0 : 200, isGuarding ? 200 : 0, 255);
            SDL_RenderFillRect(renderer, &player.rect);
        }
        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
        for (auto& o : obstacles) SDL_RenderFillRect(renderer, &o.rect);
        Uint32 now = SDL_GetTicks();
        for (auto& w : warnings) {
            if (now < w.startTime + 1000) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, rand() % 256);
                for (int i = 0; i < 5; ++i) {
                    int x0 = w.rect.x + rand() % w.rect.w;
                    SDL_RenderDrawLine(renderer, x0, w.rect.y, x0, w.rect.y + 2);
                }
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderDrawLine(renderer, w.rect.x, w.rect.y,
                                   w.rect.x + w.rect.w, w.rect.y);
            }
        }
        int hbH = 8;
        int healthY = player.rect.y - hbH - 5;
        if (isGuarding || guardPoints < maxGuardPoints) {
            int gbH = 5;
            int guardY = healthY - gbH - 3;
            SDL_Rect bg = { player.rect.x, guardY, guardBarWidth, gbH };
            SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
            SDL_RenderFillRect(renderer, &bg);
            SDL_Rect fg = { player.rect.x, guardY, displayedGuardWidth, gbH };
            SDL_SetRenderDrawColor(renderer, 0, 200, 200, 255);
            SDL_RenderFillRect(renderer, &fg);
        }
        if (showHealthBar && now - lastDamageTime <= healthBarDuration) {
            SDL_Rect bg = { player.rect.x, healthY, healthBarWidth, hbH };
            SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
            SDL_RenderFillRect(renderer, &bg);
            SDL_Rect fg = { player.rect.x, healthY, displayedHealthWidth, hbH };
            SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
            SDL_RenderFillRect(renderer, &fg);
        } else showHealthBar = false;
        if (glitchActive) {
            for (int i = 0; i < 200; ++i) {
                SDL_SetRenderDrawColor(renderer, rand() % 256, rand() % 256, rand() % 256, 150);
                SDL_Rect n = { rand() % width, rand() % height, rand() % (width / 4), rand() % 3 + 1 };
                SDL_RenderFillRect(renderer, &n);
            }
            for (int y = 0; y < height; y += 20) {
                SDL_SetRenderDrawColor(renderer, rand() % 256, rand() % 256, rand() % 256, 100);
                SDL_RenderDrawLine(renderer, 0, y + rand() % 5, width, y + rand() % 5);
            }
        }
    }

    void draw(Uint32 now) {
        if (effectActive) {
            SDL_SetRenderTarget(renderer, sceneTexture);
            SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
            SDL_RenderClear(renderer);
            drawSceneContent();
            if (effectType == 0) {
                SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
                SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA32, surf->pixels, surf->pitch);
                Uint32* p = (Uint32*)surf->pixels;
                int cnt = (surf->pitch / 4) * height;
                for (int i = 0; i < cnt; ++i) p[i] = (p[i] & 0xFF000000) | ((~p[i]) & 0x00FFFFFF);
                SDL_UpdateTexture(sceneTexture, NULL, surf->pixels, surf->pitch);
                SDL_FreeSurface(surf);
            }
            SDL_SetRenderTarget(renderer, NULL);
            SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
            SDL_RenderClear(renderer);
            if (effectType == 1)
                SDL_RenderCopyEx(renderer, sceneTexture, NULL, NULL, 0, NULL, SDL_FLIP_HORIZONTAL);
            else
                SDL_RenderCopy(renderer, sceneTexture, NULL, NULL);
            SDL_RenderPresent(renderer);
        } else {
            SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
            SDL_RenderClear(renderer);
            drawSceneContent();
            SDL_RenderPresent(renderer);
        }
    }

    void addObstacle() {
        int w = 10 + rand() % 60;
        int x = rand() % (width - w);
        int speed = 2 + rand() % 12;
        obstacles.push_back({ SDL_Rect{ x, 0, w, 10 + rand() % 40 }, speed });
    }
};

#endif // AVOID_GAME_HPP
