#ifndef FUNCTION_H
#define FUNCTION_H

#include <iostream>
#include <SDL2/SDL.h>
#include <filesystem>
#include <SDL_ttf.h>
#include <fstream>

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
