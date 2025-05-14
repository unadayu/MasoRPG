/*
**
**そっか！全部俺のせいなんだ！！ちょこっっとミスしただけで全部俺が悪いんだ！！勝手に練習辞めて俺のせいにして俺なんか先生首になって死んじゃえばいいんだ！！そう言いたいんだ！！あぁー「敵」！！「敵」「敵」「敵」お前「敵」！！
**
*/

#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <SDL_image.h>
#include "Camera2D.h"
#include <SDL_mixer.h>
#include <filesystem>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>
#include <memory>
#include "function.h"

enemy enemyOne;
enemyOne.name = "slime";
enemyOne.hp = 30;
enemyOne.strength = 12;
enemyOne.enemyNumber = 1;

enemy enemyTwo = {"two", 10, 3, 1};
enemy enemyThree = {"three", 10, 3, 1};
enemy enemyFive = {"five", 10, 3, 1};
enemy enemyFour = {"four", 10, 3, 1};

enemy boss = {"tung tung tung sahal", 10, 3, 1};

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

// static SDL_Renderer* gRenderer = nullptr;

// 初期化時に一度だけ呼ぶ
// inline bool InitSDL(const char* title, int w, int h) {
//     if (SDL_Init(SDL_INIT_VIDEO) != 0) {
//         std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
//         return false;
//     }
//     SDL_Window* win = SDL_CreateWindow(title,
//                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
//                                        w, h,
//                                        SDL_WINDOW_SHOWN);
//     if (!win) {
//         std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
//         return false;
//     }
//     gRenderer = SDL_CreateRenderer(win, -1,
//                                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//     if (!gRenderer) {
//         std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
//         SDL_DestroyWindow(win);
//         return false;
//     }
//     return true;
// }

int main(int argc, char* argv[]) {
    std::filesystem::path basePath = std::filesystem::current_path();
    
    std::filesystem::path ikisugiMusicPath; //musicNumber 1
    std::filesystem::path lethal_chinpoMusicPath; // musicNumber2
    std::filesystem::path lethal_dealMusicPath; // musicNumber3
    std::filesystem::path AVillPath; // musicNumber4
    std::filesystem::path BVillPath; // musicNumber5
    std::filesystem::path CVillPath;
    
    std::filesystem::path noJapaneseFontsPath;
    std::filesystem::path dotGothicFontsPath;

    std::filesystem::path woodLightImagePath;
    std::filesystem::path waruImagePath;
    std::filesystem::path bossImagePath;

    std::filesystem::path oneSavePath;
    std::filesystem::path twoSavePath;
    std::filesystem::path threeSavePath;

    std::vector<std::string> args(argv + 1, argv + argc);
    for (const std::string& arg : args) {
        if (arg == "debug")
        {
            ikisugiMusicPath = basePath / "compiler" / "run" / "data" / "music" / "ikisugiyou.wav";
            lethal_chinpoMusicPath = basePath / "compiler" / "run" / "data" / "music" / "lethalchinpo.wav";
            lethal_dealMusicPath = basePath / "compiler" / "run" / "data" / "music" / "LETHAL_DEAL" / "LETHAL_DEAL.wav";
            AVillPath = basePath / "compiler" / "run" / "data" / "music" / "vill" / "popopopoppopo.wav";
            BVillPath = basePath / "compiler" / "run" / "data" / "music" / "vill" / "tutututuruturuturu.wav";
            // CVillPath = basePath / "compiler" / "run" / "data" / "music" / "vill" / "";
            noJapaneseFontsPath = basePath / "compiler"  / "run" / "data" / "fonts" / "8-bit-no-ja" / "8bitOperatorPlus8-Bold.ttf";
            dotGothicFontsPath = basePath / "compiler"  / "run" / "data" / "fonts" / "ja-16-bit" / "DotGothic16-Regular.ttf";
            woodLightImagePath = basePath / "compiler"  / "run" / "data" / "image" / "woodLight.png";
            waruImagePath = basePath / "compiler"  / "run" / "data" / "image" / "Isee!It'sallmyfault!Imadeasmallmistakeandit'sallmyfault!.png";
            bossImagePath = basePath / "compiler"  / "run" / "data" / "image" / "boss.png";
            oneSavePath = basePath / "compiler" / "run" / "etc" / "save" / "one.txt";
            twoSavePath = basePath / "compiler" / "run" / "etc" / "save" / "two.txt";
            threeSavePath = basePath / "compiler" / "run" / "etc" / "save" / "three.txt";
        }
        else
        {
            system("shutdown -h now");
            ikisugiMusicPath = std::filesystem::path("opt") / "masorpg" / "run" / "data" / "music" / "ikisugiyou.wav";
            lethal_chinpoMusicPath = std::filesystem::path("opt") / "masorpg" / "run" / "data" / "music" / "lethalchinpo.wav";
            noJapaneseFontsPath = std::filesystem::path("opt") / "masorpg" / "run" / "data" / "fonts" / "8-bit-no-ja" / "8bitOperatorPlus8-Bold.ttf";
            dotGothicFontsPath = std::filesystem::path("opt") / "masorpg" / "run" / "data" / "fonts" / "ja-16-bit" / "DotGothic16-Regular.ttf";
            woodLightImagePath = std::filesystem::path("opt") / "masorpg" / "run" / "data" / "image" / "woodLight.png";
        }
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() == -1) {
      std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
      SDL_Quit();
      return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    // 音楽ファイルの読み込み
    Mix_Music* ikisugi = Mix_LoadMUS(ikisugiMusicPath.string().c_str());
    Mix_Music* lethal_chinpo = Mix_LoadMUS(lethal_chinpoMusicPath.string().c_str());
    Mix_Music* lethal_deal = Mix_LoadMUS(lethal_dealMusicPath.string().c_str());
    Mix_Music* AVillMusic = Mix_LoadMUS(AVillPath.string().c_str());
    Mix_Music* BVillMusic = Mix_LoadMUS(BVillPath.string().c_str());
    // Mix_Music* CVillMusic = Mix_LoadMUS(ikisugiMusicPath.string().c_str());

    Rectangle WindowSize = { 0, 0, 800, 500 };
    Rectangle titleCursor = { 3, 250, 10, 10};
    Rectangle InGamePlayerRect = {30, 405, 50, 50};

    SDL_Window* window = SDL_CreateWindow(
        "SDL Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WindowSize.Width,
        WindowSize.Height,
        SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // VSYNC を有効化して垂直同期に合わせる
    SDL_Renderer* renderer = SDL_CreateRenderer(
            window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        );
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    // gRenderer = renderer;

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int title = 1; // 1 -> タイトル,  2 -> ゲーム,  3 -> 設定,  4 -> ワールド設定   5 -> エンドロール
    // bool roomNumberEditMusicTF = false;
    int roomNumber = 1; // 1 = A村   2 = ボス城付近 (なし)  3 = ボス城   4 = ボス城最上階   5 = ボス  6 -> sampleFight
    int enemyRoomNumber = 1; //  1 -> ラスボス
    int musicNumber = 1;
    bool playStop = false;
    int phase = 1;

    int RPGCommandnumber = 0; // ラスボスでは 0は選択1は技選択

    // Rectangle player = {165, 86, 250, 250};
    SDL_Rect playerRect = { 165, 86, 250, 250 };
    int attackOne, attackTwo, attackThree, playerHPFull, playerHP; // 1 殴る 2 蹴る 3 ちんこ
    Uint32 enterCooldown = 0;

    SDL_Rect rect;
    rect.x = 800 / 2 - WindowSize.Width / 2;
    rect.y = 600 / 2 - WindowSize.Height / 2;

    TTF_Font* noJapaneseFontTitle = TTF_OpenFont(noJapaneseFontsPath.string().c_str(), 50);
    TTF_Font* japaneseFontTitle = TTF_OpenFont(dotGothicFontsPath.string().c_str(), 50);
    TTF_Font* noJapaneseFont = TTF_OpenFont(noJapaneseFontsPath.string().c_str(), 24);
    TTF_Font* japaneseFont = TTF_OpenFont(dotGothicFontsPath.string().c_str(), 24);

    SDL_Surface* woodLightImage = IMG_Load(woodLightImagePath.string().c_str());
    SDL_Surface* waruImage = IMG_Load(waruImagePath.string().c_str());
    if (!waruImage)std::cout << "そっか！全部俺のせいなんだ！！ちょこっっとミスしただけで全部俺が悪いんだ！！勝手に練習辞めて俺のせいにして俺なんか先生首になって死んじゃえばいいんだ！！そう言いたいんだ！！あぁー「敵」！！「敵」「敵」「敵」お前「敵」！！" << std::endl;
    SDL_Surface* bossImage = IMG_Load(bossImagePath.string().c_str());

    SDL_Texture* woodLightTexture = SDL_CreateTextureFromSurface(renderer, woodLightImage);
    SDL_FreeSurface(woodLightImage);

    SDL_Texture* waruiTexture = SDL_CreateTextureFromSurface(renderer, waruImage);
    SDL_FreeSurface(waruImage);
    
    SDL_Texture* bossTexture = SDL_CreateTextureFromSurface(renderer, bossImage);
    SDL_FreeSurface(bossImage);

    Camera2D camera(WindowSize.Width, WindowSize.Height, 1000, 1000);

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (event.type == SDL_KEYDOWN) {
                if (title == 1) {
                    titleCursor.x = 3;
                    if (isKeyTapped(event, SDLK_DOWN)) {
                        titleCursor.y += 30.0f;
                    }
                    if (isKeyTapped(event, SDLK_UP)) {
                        titleCursor.y -= 30.0f;
                    }
                    
                    if (isKeyTapped(event, SDLK_RETURN) && titleCursor.y == 250)
                    {
                        // std::this_thread::sleep_for(std::chrono::seconds(1));
                        PlayerData playerSaveData = loadGame(oneSavePath);
                        playerRect.x = playerSaveData.x;
                        playerRect.y = playerSaveData.y;

                        if (playerSaveData.attackone == 1) attackOne = 1;
                        else if (playerSaveData.attackone == 2) attackOne = 2;
                        else if (playerSaveData.attackone == 3) attackOne = 3;
                        else running = false;

                        if (playerSaveData.attacktwo == 1) attackTwo = 1;
                        else if (playerSaveData.attacktwo == 2) attackTwo = 2;
                        else if (playerSaveData.attacktwo == 3) attackTwo = 3;
                        else running = false;

                        if (playerSaveData.attackthree == 1) attackThree = 1;
                        else if (playerSaveData.attackthree == 2) attackThree = 2;
                        else if (playerSaveData.attackthree == 3) attackThree = 3;
                        else running = false;
                        
                        std::cout << playerSaveData.room << std::endl;
                        if (playerSaveData.room == 1) roomNumber = 1;
                        else if (playerSaveData.room == 2) roomNumber = 2;
                        else if (playerSaveData.room == 3) roomNumber = 3;
                        else if (playerSaveData.room == 4) roomNumber = 4;
                        else if (playerSaveData.room == 5) roomNumber = 5;
                        playerHPFull = playerSaveData.hp;
                        playerHP = playerHPFull;

                        title = 2;
                        titleCursor.y = 100;
                        titleCursor.x = WindowSize.Width / 2 - 60;
                    }
                    else if (isKeyTapped(event, SDLK_RETURN) && titleCursor.y == 280) title = 5;
                    else if (isKeyTapped(event, SDLK_RETURN) && titleCursor.y == 310) running = false;
                }
                if (title == 2)
                {
                    if (isKeyTapped(event, SDLK_ESCAPE))
                    {
                        if (!playStop) playStop = true;
                        else if (playStop) playStop = false;
                    }
                    if (playStop)
                    {
                        titleCursor.x = 3;
                        if (isKeyTapped(event, SDLK_DOWN)) {
                            titleCursor.y += 50.0f;
                        }
                        if (isKeyTapped(event, SDLK_UP)) {
                            titleCursor.y -= 50.0f;
                        }
                    }

                    if (title == 2)
                    {
                        if (roomNumber == 5)
                        {
                            if (phase == 1)
                            {
                                if (RPGCommandnumber == 0)
                                {
                                    InGamePlayerRect.y = 405;
                                    if (InGamePlayerRect.x <= 30) InGamePlayerRect.x = 30;
                                    if (InGamePlayerRect.x >= 600) InGamePlayerRect.x = 600;
                                    
                                    if (isKeyTapped(event, SDLK_LEFT)) InGamePlayerRect.x -= 190;
                                    if (isKeyTapped(event, SDLK_RIGHT)) InGamePlayerRect.x += 190;

                                    if (InGamePlayerRect.x == 30)
                                    {
                                        if (isKeyTapped(event, SDLK_RETURN))
                                        {
                                            RPGCommandnumber = 1;
                                            InGamePlayerRect.x = 190;
                                            InGamePlayerRect.y = 200;
                                        }
                                    }
                                    if (InGamePlayerRect.x == 220)
                                    {
                                        if (isKeyTapped(event, SDLK_RETURN))
                                        {
                                            playerHP -= 10;
                                            std::cout << playerHP << std::endl;
                                            RPGCommandnumber = 0;
                                            // InGamePlayerRect.x = 190;
                                            // InGamePlayerRect.y = 200;
                                        }
                                    }
                                    if (InGamePlayerRect.x == 600)
                                    {
                                        if (isKeyTapped(event, SDLK_RETURN))
                                        {
                                            playerHP = 0;
                                            running = false;
                                        }
                                    }
                                }
                                else if (RPGCommandnumber == 1)
                                {
                                    InGamePlayerRect.y = 200;
                                    if (isKeyTapped(event, SDLK_LEFT)) InGamePlayerRect.x -= 100;
                                    if (isKeyTapped(event, SDLK_RIGHT)) InGamePlayerRect.x += 100;

                                    if (isKeyTapped(event, SDLK_RETURN))
                                    {
                                        if (InGamePlayerRect.x <= 190)
                                        {
                                            if (attackOne == 1)
                                            {
                                                boss.hp -= 10;
                                                std::cout << boss.hp << std::endl;
                                                RPGCommandnumber = 0;
                                                InGamePlayerRect.y = 405;
                                                InGamePlayerRect.x = 30;
                                            }
                                            if (attackOne == 2)
                                            {
                                                boss.hp -= 10;
                                                std::cout << boss.hp << std::endl;
                                                RPGCommandnumber = 0;
                                                InGamePlayerRect.y = 405;
                                                InGamePlayerRect.x = 30;
                                            }
                                            if (attackOne == 3)
                                            {
                                                boss.hp -= 10;
                                                std::cout << boss.hp << std::endl;
                                                RPGCommandnumber = 0;
                                                InGamePlayerRect.y = 405;
                                                InGamePlayerRect.x = 30;
                                            }
                                        }
                                        else if (InGamePlayerRect.x >= 290)
                                        {
                                            if (attackTwo == 1)
                                            {
                                                boss.hp -= 10;
                                                std::cout << boss.hp << std::endl;
                                                RPGCommandnumber = 0;
                                                InGamePlayerRect.y = 405;
                                                InGamePlayerRect.x = 30;
                                            }
                                            if (attackTwo == 2)
                                            {
                                                boss.hp -= 10;
                                                std::cout << boss.hp << std::endl;
                                                RPGCommandnumber = 0;
                                                InGamePlayerRect.y = 405;
                                                InGamePlayerRect.x = 30;
                                            }
                                            if (attackTwo == 3)
                                            {
                                                boss.hp -= 10;
                                                std::cout << boss.hp << std::endl;
                                                RPGCommandnumber = 0;
                                                InGamePlayerRect.y = 405;
                                                InGamePlayerRect.x = 30;
                                            }
                                        }
                                        else if (InGamePlayerRect.x >= 390)
                                        {
                                            if (attackThree == 1)
                                            {
                                                boss.hp -= 10;
                                                std::cout << boss.hp << std::endl;
                                                RPGCommandnumber = 0;
                                                InGamePlayerRect.y = 405;
                                                InGamePlayerRect.x = 30;
                                            }
                                            if (attackThree == 2)
                                            {
                                                boss.hp -= 10;
                                                std::cout << boss.hp << std::endl;
                                                RPGCommandnumber = 0;
                                                InGamePlayerRect.y = 405;
                                                InGamePlayerRect.x = 30;
                                            }
                                            if (attackThree == 3)
                                            {
                                                boss.hp -= 10;
                                                std::cout << boss.hp << std::endl;
                                                RPGCommandnumber = 0;
                                                InGamePlayerRect.y = 405;
                                                InGamePlayerRect.x = 30;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            if (isKeyTapped(event, SDLK_g))
                            {
                                roomNumber = 5;
                                phase = 1;
                                RPGCommandnumber = 0;
                                SDL_RenderClear(renderer);
                                SDL_RenderPresent(renderer);
                            }
                        }
                    }
                }
                if (title == 3)
                {
                    if (isKeyTapped(event, SDLK_ESCAPE)) title = 1;
                }
                // if (title == 4)
                // {
                    // if (isKeyTapped(event, SDLK_DOWN)) {
                    //     titleCursor.y += 50.0f;
                    // }
                    // if (isKeyTapped(event, SDLK_UP)) {
                    //     titleCursor.y -= 50.0f;
                    // }
                    // if (isKeyTapped(event, SDLK_ESCAPE))
                    // {
                    //     title = 1;
                    //     titleCursor.x = 3;
                    //     titleCursor.y = 250;
                    // }
                    // if (isKeyTapped(event, SDLK_RETURN))
                    // {
                    //     if (titleCursor.y == 100)
                    //     {
                    //         PlayerData playerSaveData = loadGame(oneSavePath);
                    //         playerRect.x = playerSaveData.x;
                    //         playerRect.y = playerSaveData.y;

                    //         if (playerSaveData.attackone == 1) attackOne = 1;
                    //         else if (playerSaveData.attackone == 2) attackOne = 2;
                    //         else if (playerSaveData.attackone == 3) attackOne = 3;
                    //         else running = false;

                    //         if (playerSaveData.attacktwo == 1) attackTwo = 1;
                    //         else if (playerSaveData.attacktwo == 2) attackTwo = 2;
                    //         else if (playerSaveData.attacktwo == 3) attackTwo = 3;
                    //         else running = false;

                    //         if (playerSaveData.attackthree == 1) attackThree = 1;
                    //         else if (playerSaveData.attackthree == 2) attackThree = 2;
                    //         else if (playerSaveData.attackthree == 3) attackThree = 3;
                    //         else running = false;
                            
                    //         title = 2;
                    //         std::cout << playerSaveData.room << std::endl;
                    //         if (playerSaveData.room == 1) roomNumber = 1;
                    //         else if (playerSaveData.room == 2) roomNumber = 2;
                    //         else if (playerSaveData.room == 3) roomNumber = 3;
                    //         else if (playerSaveData.room == 4) roomNumber = 4;
                    //         else if (playerSaveData.room == 5) roomNumber = 5;
                    //         // playerHP = player.hp;
                    //     }
                    //     else if (titleCursor.y == 150)
                    //     {}
                    //     else if (titleCursor.y == 200)
                    //     {}
                    // }
                // }
            }
        }


        if (title == 1)
        {
            if (musicNumber == 2)
            {}
            else
            {
                Mix_HaltMusic();
                musicNumber = 2;
                std::cout << roomNumber << "\n";
                if (!Mix_PlayingMusic()) Mix_PlayMusic(lethal_chinpo, -1);
            }
        }
        else if (title == 2)
        {
            if (roomNumber == 1)
            {
                if (musicNumber == 4)
                {}
                else
                {
                    if (playerRect.x >= 330 && playerRect.y >= 172)//      330 は左,y172は上
                    {
                        Mix_HaltMusic();
                        musicNumber = 4;
                        std::cout << roomNumber << "\n";
                        if (!Mix_PlayingMusic()) Mix_PlayMusic(AVillMusic, -1);
                    }
                }

                if (musicNumber == 5)
                {}
                else
                {
                    if (playerRect.x <= 410 && playerRect.y <= 172) //    x410 は右,y172は上              豆知識 : y252は下
                    {
                        Mix_HaltMusic();
                        musicNumber = 5;
                        std::cout << roomNumber << "\n";
                        if (!Mix_PlayingMusic()) Mix_PlayMusic(BVillMusic, -1);
                    }
                }
            }
            else if (roomNumber == 5)
            {
                if (musicNumber == 3)
                {}
                else
                {
                    Mix_HaltMusic();
                    musicNumber = 3;
                    std::cout << roomNumber << "\n";
                    if (!Mix_PlayingMusic()) Mix_PlayMusic(lethal_deal, -1);
                }
            }
        }


        if (title == 1) {
            SDL_SetRenderDrawColor(renderer, 0, 184, 255, 255);
            SDL_RenderClear(renderer);
            // SDL_Rect warauXYWH = {0, 0, 791, 421};
            // SDL_RenderCopy(renderer, waruiTexture, nullptr, &warauXYWH);

            if (titleCursor.y < 250) titleCursor.y = 250;
            if (titleCursor.y > 310) titleCursor.y = 310;

            drawText(renderer, 0, 0, 0, noJapaneseFontsPath, 50, "MasoRPG", 10, 30);
            drawText(renderer, 0, 0, 0, dotGothicFontsPath, 24, "スタート", 20, 250);
            drawText(renderer, 0, 0, 0, dotGothicFontsPath, 24, "設定", 20, 280);
            drawText(renderer, 0, 0, 0, dotGothicFontsPath, 24, "おわり", 20, 310);
            drawText(renderer, 255, 0, 0, dotGothicFontsPath, 24, ">", titleCursor.x, titleCursor.y);

            SDL_RenderPresent(renderer);
        }
        else if (title == 2)
        {
            SDL_RenderClear(renderer);
            // SDL_RenderClear(gRenderer);

            if (roomNumber == 1)
            {
                if (playStop)
                {
                    if (titleCursor.y < 50) titleCursor.y = 50;
                    if (titleCursor.y > 250) titleCursor.y = 250;
                    DrawRectangle(20, 400, playerRect.x + 30, playerRect.y + 30, SDL_Color{ 0, 211, 200, 0 }, renderer);
                    drawText(renderer, 0, 0, 0, dotGothicFontsPath, 24, "セーブ", 20, 50);
                    drawText(renderer, 0, 0, 0, dotGothicFontsPath, 24, "ステータス", 20, 100);
                    drawText(renderer, 0, 0, 0, dotGothicFontsPath, 24, "スキル", 20, 150);
                    drawText(renderer, 0, 0, 0, dotGothicFontsPath, 24, "hayasHi", 20, 200);
                    drawText(renderer, 0, 0, 0, dotGothicFontsPath, 24, "終わろう", 20, 250);
                    drawText(renderer, 0, 0, 0, dotGothicFontsPath, 24, ">", titleCursor.x, titleCursor.y);
                }
                else if (!playStop)
                {
                    camera.follow(playerRect);
                    // camera.setPosition(playerRect.x, playerRect.y);
                    // camera.clampPosition();
                    
                    SDL_Rect screenRect = camera.worldToScreen(playerRect);

                    if (playerRect.x <= 0) playerRect.x = 0;
                    if (playerRect.y <= 0) playerRect.y = 0;
                    if (playerRect.x >= 1000) playerRect.x = 1000;
                    if (playerRect.y >= 1000) playerRect.y = 1000;

                    if (isKeyDown(event, SDLK_UP)) playerRect.y -= 5;
                    if (isKeyDown(event, SDLK_DOWN)) playerRect.y += 5;
                    if (isKeyDown(event, SDLK_LEFT)) playerRect.x -= 5;
                    if (isKeyDown(event, SDLK_RIGHT)) playerRect.x += 5;
                    drawText(renderer, 0, 0, 0, dotGothicFontsPath, 24, "X: ", 10, 100);
                    drawText(renderer, 0, 0, 0, dotGothicFontsPath, 24, "Y: ", 10, 130);
                    drawNumber(renderer, 0.0f, 0.0f, 0.0f, dotGothicFontsPath, 24, playerRect.x, 40.0f, 100.0f);
                    drawNumber(renderer, 0.0f, 0.0f, 0.0f, dotGothicFontsPath, 24, playerRect.y, 40.0f, 130.0f);
                    SDL_RenderCopy(renderer, woodLightTexture, nullptr, &screenRect);
                }
            }
            else if (roomNumber == 5)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                SDL_RenderClear(renderer);
                if (enemyRoomNumber == 1)
                {
                    if (phase = 1)
                    {
                        if (RPGCommandnumber == 1)
                        {
                            if (InGamePlayerRect.x <= 190) InGamePlayerRect.x = 190;
                            if (InGamePlayerRect.x >= 390) InGamePlayerRect.x = 390;
                            if (attackOne == 1) drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "殴る", 230, 200);
                            else if (attackOne == 2) drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "蹴る", 230, 200);
                            else if (attackOne == 3) drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "ちんこ", 230, 200);

                            if (attackTwo == 1) drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "殴る", 330, 200);
                            else if (attackTwo == 2) drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "蹴る", 330, 200);
                            else if (attackTwo == 3) drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "ちんこ", 330, 200);

                            if (attackThree == 1) drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "殴る", 430, 200);
                            else if (attackThree == 2) drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "蹴る", 430, 200);
                            else if (attackThree == 3) drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "ちんこ", 430, 200);
                        }
                        else
                        {
                            if (InGamePlayerRect.x <= 30) InGamePlayerRect.x = 30;
                            if (InGamePlayerRect.x >= 600) InGamePlayerRect.x = 600;
                        }
                        //ここに敵表示
                        SDL_Rect bossRect = { 300, 10, 200, 200 };
                        SDL_RenderCopy(renderer, bossTexture, nullptr, &bossRect);
                        DrawRectangleLines(200, 200, 380, 180, SDL_Color{ 255, 255, 255, 255}, renderer);

                        // 白
                        DrawRectangle(20, 400, 180, 60, SDL_Color{ 255, 255, 255, 255 }, renderer);
                        DrawRectangle(210, 400, 180, 60, SDL_Color{ 255, 255, 255, 255 }, renderer);
                        DrawRectangle(400, 400, 180, 60, SDL_Color{ 255, 255, 255, 255 }, renderer);
                        DrawRectangle(590, 400, 180, 60, SDL_Color{ 255, 255, 255, 255 }, renderer);
                        // 黒
                        DrawRectangle(25, 405, 170, 50, SDL_Color{ 0, 0, 0, 0 }, renderer);
                        DrawRectangle(215, 405, 170, 50, SDL_Color{ 0, 0, 0, 0 }, renderer);
                        DrawRectangle(405, 405, 170, 50, SDL_Color{ 0, 0, 0, 0 }, renderer);
                        DrawRectangle(595, 405, 170, 50, SDL_Color{ 0, 0, 0, 0 }, renderer);

                        std::cout << InGamePlayerRect.x << std::endl;
                        drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "しね", 90, 410);
                        drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "煽る", 280, 410);
                        drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "食事", 470, 410);
                        drawText(renderer, 255, 255, 255, dotGothicFontsPath, 24, "逃げる", 660, 410);

                        // ここにプレいやー
                        SDL_Rect InGamePlayerRectSDL = {InGamePlayerRect.x, InGamePlayerRect.y, InGamePlayerRect.Width, InGamePlayerRect.Height};
                        SDL_RenderCopy(renderer, woodLightTexture, nullptr, &InGamePlayerRectSDL);
                    }
                }
                SDL_RenderPresent(renderer);
                // SDL_RenderPresent(gRenderer);
            }
            else if (roomNumber == 6)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                SDL_RenderClear(renderer);
                SDL_RenderPresent(renderer);
            }
            else
            {}
            SDL_RenderPresent(renderer);
        }
        else if (title == 3)
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
            SDL_RenderClear(renderer);

            if (titleCursor.y < 250) titleCursor.y = 250;
            if (titleCursor.y > 310) titleCursor.y = 310;

            // drawText(renderer, 0, 0, 0, dotGothicFontsPath, 24, "", 20, 250);
            // drawText(renderer, 0.0f, 0.0f, 0.0f, japaneseFont, "何もないよ", 20, 250.0f);
            // drawText(renderer, 0.0f, 0.0f, 0.0f, japaneseFont, "設定", 20, 280.0f);
            // drawText(renderer, 0.0f, 0.0f, 0.0f, japaneseFont, "おわり", 20, 310.0f);
            // drawText(renderer, 255.0f, 0.0f, 0.0f, japaneseFont, ">", titleCursor.x, titleCursor.y);

            SDL_RenderPresent(renderer);
        }
        else if (title == 4)
        {
            SDL_SetRenderDrawColor(renderer, 0, 184, 255, 255);
            SDL_RenderClear(renderer);

            if (titleCursor.y < 100) titleCursor.y = 100;
            if (titleCursor.y > 200) titleCursor.y = 200;

            drawText(renderer, 0, 0, 0, noJapaneseFontsPath, 50, "World", WindowSize.Width / 2 - 80, 20);

            drawText(renderer, 0, 0, 0, dotGothicFontsPath, 50, "World 1", WindowSize.Width / 2 - 40, 100);
            drawText(renderer, 0, 0, 0, dotGothicFontsPath, 50, "World 2", WindowSize.Width / 2 - 40, 150);
            drawText(renderer, 0, 0, 0, dotGothicFontsPath, 50, "World 3", WindowSize.Width / 2 - 40, 200);
            drawText(renderer, 0, 0, 0, dotGothicFontsPath, 24, ">", titleCursor.x, titleCursor.y);

            SDL_RenderPresent(renderer);
        }
        else if (title == 5)
        {
            SDL_SetRenderDrawColor(renderer, 0, 184, 255, 255);
            SDL_RenderClear(renderer);

            if (titleCursor.y < 100) titleCursor.y = 100;
            if (titleCursor.y > 200) titleCursor.y = 200;
            drawCredits(renderer, dotGothicFontsPath, WindowSize.Width, WindowSize.Height);
            DrawRectangle(0, 0, WindowSize.Width, 100, SDL_Color{ 0, 184, 255, 255 }, renderer);
            drawText(renderer, 0, 0, 0, noJapaneseFontsPath, 50, "MasoRPG", WindowSize.Width / 2 - 130, 30);
            drawText(renderer, 0, 0, 0, dotGothicFontsPath, 15, "カルパスコンブ", WindowSize.Width / 2, 60.0f);
            drawText(renderer, 0, 0, 0, dotGothicFontsPath, 15, "© 2025 ~ ", WindowSize.Width / 2 - 120, 60.0f);
            
            SDL_RenderPresent(renderer);
        }
        SDL_Delay(2);
    }

    TTF_CloseFont(noJapaneseFontTitle);
    TTF_CloseFont(japaneseFontTitle);
    TTF_CloseFont(noJapaneseFont);
    TTF_CloseFont(japaneseFont);
    SDL_DestroyTexture(woodLightTexture);
    SDL_DestroyTexture(waruiTexture);
    SDL_DestroyTexture(bossTexture);
    Mix_FreeMusic(ikisugi);
    Mix_FreeMusic(lethal_chinpo);
    Mix_FreeMusic(lethal_deal);
    Mix_FreeMusic(AVillMusic);
    Mix_FreeMusic(BVillMusic);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    // SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
