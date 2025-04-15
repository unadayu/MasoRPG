#pragma once
#include <SDL2/SDL.h>

class Camera2D {
public:
    int x, y, w, h;
    int mapWidth, mapHeight;

    Camera2D(int screenWidth, int screenHeight, int mapWidth, int mapHeight);

    void follow(const SDL_Rect& target);

    SDL_Rect getView() const;

    SDL_Point worldToScreen(int worldX, int worldY) const;
    SDL_Rect worldToScreen(const SDL_Rect& worldRect) const;
};
