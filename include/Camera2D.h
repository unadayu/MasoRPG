// Camera2D.h
#ifndef CAMERA2D_H
#define CAMERA2D_H

#include <SDL2/SDL.h>

class Camera2D {
public:
    // screenWidth, screenHeight だけを指定（mapWidth/mapHeight は後から set する場合）
    Camera2D(int width, int height);

    // screenWidth, screenHeight, mapWidth, mapHeight を同時に指定
    Camera2D(int screenWidth, int screenHeight, int mapWidth, int mapHeight);

    // カメラをターゲットに追従させる
    void follow(const SDL_Rect& target);

    // ワールド座標を画面座標に変換する
    SDL_Rect worldToScreen(const SDL_Rect& worldRect) const;

    // カメラの位置を直接設定（必要に応じて）
    void setPosition(int x, int y);

    // カメラのビューポート（表示領域）を取得
    SDL_Rect getView() const;

private:
    int screenWidth;   // 画面幅
    int screenHeight;  // 画面高さ
    int mapWidth;      // マップ全体の幅
    int mapHeight;     // マップ全体の高さ
    int x;             // カメラ左上の X 座標
    int y;             // カメラ左上の Y 座標
    int viewX;         // 未使用（将来拡張用）
    int viewY;         // 未使用（将来拡張用）

    // マップ範囲外へカメラが出ないように位置を制限
    void clampPosition();
};

#endif // CAMERA2D_H
