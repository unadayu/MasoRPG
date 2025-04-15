#include "Camera2D.h"

Camera2D::Camera2D(int screenWidth, int screenHeight, int mapWidth, int mapHeight)
    : x(0), y(0), w(screenWidth), h(screenHeight), mapWidth(mapWidth), mapHeight(mapHeight) {}

void Camera2D::follow(const SDL_Rect& target) {
    x = target.x + target.w / 2 - w / 2;
    y = target.y + target.h / 2 - h / 2;

    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > mapWidth - w) x = mapWidth - w;
    if (y > mapHeight - h) y = mapHeight - h;
}

SDL_Rect Camera2D::getView() const {
    return SDL_Rect{x, y, w, h};
}

SDL_Point Camera2D::worldToScreen(int worldX, int worldY) const {
    return SDL_Point{worldX - x, worldY - y};
}

SDL_Rect Camera2D::worldToScreen(const SDL_Rect& worldRect) const {
    return SDL_Rect{
        worldRect.x - x,
        worldRect.y - y,
        worldRect.w,
        worldRect.h
    };
}
