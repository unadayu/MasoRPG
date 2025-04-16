#include "Camera2D.h"

// コンストラクタ
Camera2D::Camera2D(int screenWidth, int screenHeight, int mapWidth, int mapHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight),
      mapWidth(mapWidth), mapHeight(mapHeight),
      x(0), y(0), w(screenWidth), h(screenHeight) {}

// カメラをターゲットに追従させる
void Camera2D::follow(const SDL_Rect& target) {
    x = target.x + target.w / 2 - screenWidth / 2;
    y = target.y + target.h / 2 - screenHeight / 2;
}

// 現在のカメラの視界を返す
SDL_Rect Camera2D::getView() const {
    SDL_Rect view = { x, y, w, h };
    return view;
}

// ワールド座標をスクリーン座標に変換
SDL_Rect Camera2D::worldToScreen(const SDL_Rect& worldRect) const {
    SDL_Rect screenRect;
    screenRect.x = worldRect.x - x;
    screenRect.y = worldRect.y - y;
    screenRect.w = worldRect.w;
    screenRect.h = worldRect.h;
    return screenRect;
}
