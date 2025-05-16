#ifndef FUNCTION_H
#define FUNCTION_H

#include <iostream>
#include <SDL2/SDL.h>
#include <filesystem>
#include <SDL_ttf.h>
#include <fstream>
#include <vector>

struct Rectangle {
    int x;
    int y;
    int Width;
    int Height;
};

struct PlayerData {
    int x;
    int y;
    int room;
    int hp;
    int attackone;
    int attacktwo;
    int attackthree;
};

struct enemy {
    std::string name;
    int hp;
    int strength;
    int enemyNumber;
    int level;
};

struct Line {
    std::string text;
    int fontSize;    // 役割は大きく(40)、名前は小さく(25)
};

// テキスト描画関数
void drawText(SDL_Renderer* renderer, float R, float G, float B, const std::filesystem::path& fontPath, int fontSize, const char* Text, float x, float y)
{
    TTF_Font* font = TTF_OpenFont(fontPath.string().c_str(), fontSize);
    if (!font) {
    std::cerr << "フォントの読み込みに失敗しました: " << TTF_GetError() << std::endl;
    return;
    }

    SDL_Color color = {static_cast<Uint8>(R), static_cast<Uint8>(G), static_cast<Uint8>(B)};
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, Text, color);
    if (!surface) {
    std::cerr << "テキストのレンダリングに失敗しました: " << TTF_GetError() << std::endl;
    TTF_CloseFont(font);
    return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    int w = surface->w;
    int h = surface->h;
    SDL_FreeSurface(surface);

    SDL_Rect dstRect = {static_cast<int>(x), static_cast<int>(y), w, h};
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

void drawNumber(SDL_Renderer* renderer, float R, float G, float B, std::filesystem::path& font, int fontSize ,int number, float x, float y)
{
    // 数字を文字列に変換
    std::string str = std::to_string(number);

    // drawText に渡す（文字列を .c_str() で const char* に変換）
    drawText(renderer, R, G, B, font, fontSize, str.c_str(), x, y);
}

// スクロール付きエンドロール描画
void drawCredits(SDL_Renderer* renderer, const std::string& fontPath, int screenWidth, int screenHeight)
{
    // ---- static 変数で一度だけ初期化・経過時間管理 ----
    static float scrollOffset = 0.0f;                       // 累積スクロール量
    static Uint32 lastTicks = SDL_GetTicks();               // 前フレーム時刻

    Uint32 now = SDL_GetTicks();
    float dt = (now - lastTicks) / 1000.0f;                  // 秒単位の差分時間
    lastTicks = now;

    const float speed = 50.0f;                              // px/秒
    scrollOffset += speed * dt;

    // ---- 表示するすべての行を平坦化して作成 ----
    std::vector<Line> lines;
    auto pushRole = [&](const char* role){
        lines.push_back({role, 30});
    };
        auto pushName = [&](const char* name){
        lines.push_back({name, 20});
    };
    // CreditEntry 相当の処理
    pushRole("プロデューサー(GitHubのid)");
    pushName("RainbowPuiPuiMolcar");
    pushName("hamutaro1221(旧MeimaruNishimura328)");
    pushRole("奴隷(GitHubのid)");
    pushName("Hamster-crab");
    pushRole("デザイナー(GitHubのid)");
    pushName("RainbowPuiPuiMolcar");
    pushName("hamutaro1221(旧MeimaruNishimura328)");
    pushName("沼(GitHubアカウント無し)");
    pushRole("音楽(GitHubのid)");
    pushName("RainbowPuiPuiMolcar");
    pushName("Hamster-crab");
    pushName("unadayu");
    pushRole("ストーリー担当(gGitHubのid)");
    pushName("RainbowPuiPuiMolcar");
    pushName("hamutaro1221(旧MeimaruNishimura328)");
    pushRole("テスター(GitHubのid)");
    pushName("いない");

    // ---- 描画 ----
    const float lineSpacing = 35.0f;
    // スタート位置は画面下端から少しだけ下
    float baseY = static_cast<float>(screenHeight) + 20.0f;

    for (size_t i = 0; i < lines.size(); ++i) {
        float y = baseY + i * lineSpacing - scrollOffset;
        // 完全に画面外（上端）なら飛ばす
        if (y < -lineSpacing) continue;
        // まだ画面下（スクロールが追いついていない）ならも描くか判定不要なら描画
        // フォントサイズに応じて描画
        drawText(renderer, 0, 0, 0, fontPath, lines[i].fontSize, lines[i].text.c_str(), 10, y);
    }
}

PlayerData loadGame(const std::string& filename) {
    PlayerData data{0, 0, 0, 100, 0, 0, 0}; // デフォルト値
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        if (std::getline(iss, key, '=')) {
            std::string valueStr;
            if (std::getline(iss, valueStr)) {
                int value = std::stoi(valueStr);
                if (key == "x") data.x = value;
                else if (key == "y") data.y = value;
                else if (key == "room") data.room = value;
                else if (key == "hp") data.hp = value;
                else if (key == "attackone") data.attackone = value;
                else if (key == "attacktwo") data.attacktwo = value;
                else if (key == "attackthree") data.attackthree = value;
            }
        }
    }

    return data;
}

void saveGame(const PlayerData& data, const std::filesystem::path& filepath) {
    std::ofstream file(filepath);
    if (!file) {
        // ファイルオープン失敗時の処理
        throw std::runtime_error("Failed to open file for writing: " + filepath.string());
    }

    file << "x=" << data.x << '\n';
    file << "y=" << data.y << '\n';
    file << "room=" << data.room << '\n';
    file << "hp=" << data.hp << '\n';
    file << "attackone=" << data.attackone << '\n';
    file << "attacktwo=" << data.attacktwo << '\n';
    file << "attackthree=" << data.attackthree << '\n';
}

// 長押しによるリピートを無視し、純粋に“キーを押した瞬間だけ”を検知する関数
bool isKeyTapped(const SDL_Event& event, SDL_Keycode key) {
    return event.type == SDL_KEYDOWN
        && event.key.keysym.sym == key
        && event.key.repeat == 0;  // repeat==0: 長押しの二回目以降を除外
}

bool isKeyDown(SDL_Event& event, SDL_Keycode key) {
    return (event.type == SDL_KEYDOWN && event.key.keysym.sym == key);
}

// raylib ライクな一行描画関数
inline void DrawRectangle(int x, int y, int w, int h, SDL_Color c, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_Rect r{ x, y, w, h };
    SDL_RenderFillRect(renderer, &r);
}

// 同じく枠線だけ
inline void DrawRectangleLines(int x, int y, int w, int h, SDL_Color c, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_Rect r{ x, y, w, h };
    SDL_RenderDrawRect(renderer, &r);
}

#endif // FUNCTION_H
