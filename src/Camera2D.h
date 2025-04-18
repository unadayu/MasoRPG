#ifndef CAMERA2D_H
#define CAMERA2D_H

#include <SDL2/SDL.h>

class Camera2D {
public:
    Camera2D(int width, int height);
    Camera2D(int screenWidth, int screenHeight, int mapWidth, int mapHeight);

    // カメラをターゲットに追従させる
    void follow(const SDL_Rect& target);

    // ワールド座標を画面座標に変換する
    SDL_Rect worldToScreen(const SDL_Rect& worldRect) const;

    // カメラの位置設定
    void setPosition(int x, int y);

    // カメラの位置を制限する
    void clampPosition(int mapWidth, int mapHeight);

    // カメラのビューポート（画面上でのカメラの位置）を取得する
    SDL_Rect getView() const;

private:
    int screenWidth, screenHeight;  // 画面サイズ
    int mapWidth, mapHeight;        // マップのサイズ
    int x, y;                       // カメラの位置
    int viewX, viewY;               // ビューポートの位置
};

#endif
