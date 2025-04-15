#pragma once
#include <SDL2/SDL.h>

class Camera2D {
    public:
        int x, y, w, h;
        int mapWidth, mapHeight;
    
        Camera2D(int screenWidth, int screenHeight, int mapWidth, int mapHeight)
            : x(0), y(0), w(screenWidth), h(screenHeight), mapWidth(mapWidth), mapHeight(mapHeight) {}
    
        void follow(const SDL_Rect& target) {
            x = target.x + target.w / 2 - w / 2;
            y = target.y + target.h / 2 - h / 2;
    
            if (x < 0) x = 0;
            if (y < 0) y = 0;
            if (x > mapWidth - w) x = mapWidth - w;
            if (y > mapHeight - h) y = mapHeight - h;
        }
    
        SDL_Rect worldToScreen(const SDL_Rect& worldRect) const {
            return SDL_Rect{
                worldRect.x - x,
                worldRect.y - y,
                worldRect.w,
                worldRect.h
            };
        }
    };
    
