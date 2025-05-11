// Camera2D.cpp
#include "Camera2D.h"

Camera2D::Camera2D(int width, int height) 
    : screenWidth(width)
    , screenHeight(height)
    , mapWidth(0)
    , mapHeight(0)
    , x(0)
    , y(0)
    , viewX(0)
    , viewY(0)
{}

Camera2D::Camera2D(int screenWidth, int screenHeight, int mapWidth, int mapHeight) 
    : screenWidth(screenWidth)
    , screenHeight(screenHeight)
    , mapWidth(mapWidth)
    , mapHeight(mapHeight)
    , x(0)
    , y(0)
    , viewX(0)
    , viewY(0)
{}

void Camera2D::follow(const SDL_Rect& target) {
    // プレイヤー中心を画面中央に置くように仮のカメラ座標を計算
    int targetCenterX = target.x + target.w / 2;
    int targetCenterY = target.y + target.h / 2;

    x = targetCenterX - screenWidth / 2;
    y = targetCenterY - screenHeight / 2;
}

SDL_Rect Camera2D::worldToScreen(const SDL_Rect& worldRect) const {
    SDL_Rect screenRect;
    screenRect.x = worldRect.x - x;
    screenRect.y = worldRect.y - y;
    screenRect.w = worldRect.w;
    screenRect.h = worldRect.h;
    return screenRect;
}

void Camera2D::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
    clampPosition();
}

void Camera2D::clampPosition() {
    if (x < 0)                        x = 0;
    if (y < 0)                        y = 0;
    if (x + screenWidth > mapWidth)   x = mapWidth  - screenWidth;
    if (y + screenHeight > mapHeight) y = mapHeight - screenHeight;
}

SDL_Rect Camera2D::getView() const {
    return SDL_Rect{ x, y, screenWidth, screenHeight };
}
