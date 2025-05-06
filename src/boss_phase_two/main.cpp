#include "AvoidGame.hpp"

int main(int argc, char* argv[]) {
    // 画面サイズ 640x480, 体力バー長 5
    AvoidGame game(640, 480, 5);

    if (!game.init("Avoid Game")) {
        SDL_Log("SDL 初期化失敗: %s", SDL_GetError());
        return -1;
    }

    // プレイヤー用画像を指定（PNG 推奨）
    if (!game.loadPlayerTexture("compiler/run/data/image/boss.png")) {
        SDL_Log("テクスチャ読み込み失敗: %s", SDL_GetError());
        // texture がない場合は矩形で描画されます
    }

    game.run();
    return 0;
}
