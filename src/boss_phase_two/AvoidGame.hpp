#ifndef AVOID_GAME_HPP
#define AVOID_GAME_HPP

#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

struct Player {
    SDL_Rect rect;
    int speed;
};

struct Obstacle {
    SDL_Rect rect;
    int speed;
};

class AvoidGame {
public:
    AvoidGame(int w, int h)
        : width(w), height(h), window(nullptr), renderer(nullptr),
          running(false), health(3) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }

    ~AvoidGame() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    bool init(const char* title) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
        if (!window) return false;
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) return false;

        player.rect = { width / 2 - 25, height - 60, 50, 50 };
        player.speed = 5;

        return true;
    }

    void run() {
        running = true;
        Uint32 lastTime = SDL_GetTicks();
        while (running) {
            handleEvents();
            update();
            draw();

            Uint32 now = SDL_GetTicks();
            if (now - lastTime > 1000) {
                addObstacle();
                lastTime = now;
            }

            SDL_Delay(16);
        }
    }

private:
    int width, height;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Player player;
    std::vector<Obstacle> obstacles;
    bool running;
    int health;

    void handleEvents() {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
        }

        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_LEFT] && player.rect.x > 0)
            player.rect.x -= player.speed;
        if (keystate[SDL_SCANCODE_RIGHT] && player.rect.x + player.rect.w < width)
            player.rect.x += player.speed;
    }

    void update() {
        for (auto& obs : obstacles) {
            obs.rect.y += obs.speed;
        }

        obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(),
            [this](const Obstacle& o) { return o.rect.y > height; }), obstacles.end());

        for (auto& obs : obstacles) {
            if (SDL_HasIntersection(&player.rect, &obs.rect)) {
                health--;
                obs.rect.y = height + 1; // 画面外に飛ばす
                if (health <= 0) {
                    running = false;
                }
            }
        }
    }

    void draw() {
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // プレイヤー
        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
        SDL_RenderFillRect(renderer, &player.rect);

        // 障害物
        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
        for (const auto& obs : obstacles) {
            SDL_RenderFillRect(renderer, &obs.rect);
        }

        // 体力表示
        for (int i = 0; i < health; ++i) {
            SDL_Rect hp = { 10 + i * 25, 10, 20, 20 };
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &hp);
        }

        SDL_RenderPresent(renderer);
    }

    void addObstacle() {
        int w = 20 + rand() % 80;
        int h = 10 + rand() % 30;
        int x = rand() % (width - w);
        obstacles.push_back({ SDL_Rect{x, 0, w, h}, 4 + rand() % 3 });
    }
};

#endif // AVOID_GAME_HPP
