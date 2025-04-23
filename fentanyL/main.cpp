#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "./fentanyL/raygui.h"

#include <cstdlib>
#include <filesystem>

namespace fs = std::filesystem;

void runCommand(const char* cmd) {
    std::system(cmd);
}

void bootstrap() {
    runCommand("g++ --version > /dev/null 2>&1 || sudo apt update && sudo apt install -y g++");
}

void build(fs::path compilerPath) {
    std::string cmd = "g++ -std=c++17 -o compiler/run/bin/main src/main.cpp src/Camera2D.cpp $(pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer)";
    runCommand(cmd.c_str());
}

void run() {
    runCommand("compiler/run/bin/main debug");
}

void yajuiku() {
    runCommand("g++ -std=c++17 compiler/src/main.cpp -o yajuiku");
}

void install(fs::path compilerPath) {
    runCommand(("sudo cp -r " + compilerPath.string() + " /opt/masoRpgDebugData").c_str());
}

void remove() {
    runCommand("sudo rm -rf /opt/masoRpgDebugData");
}

void ruun() {
    runCommand("./opt/masorpg/bin/main");
}

void reset(fs::path compilerPath) {
    fs::path runPath = compilerPath / "run";
    fs::path samplePath = compilerPath / "sample";
    if (fs::exists(runPath)) fs::remove_all(runPath);
    fs::copy(samplePath, runPath, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
}

int main() {
    fs::path basePath = fs::current_path();
    fs::path compilerPath = basePath / "compiler";

    InitWindow(800, 600, "Yajuiku GUI");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Yajuiku GUI", 10, 10, 30, BLACK);

        if (GuiButton((Rectangle){ 50, 60, 200, 30 }, "bootstrap")) bootstrap();
        if (GuiButton((Rectangle){ 50, 100, 200, 30 }, "build")) { reset(compilerPath); build(compilerPath); }
        if (GuiButton((Rectangle){ 50, 140, 200, 30 }, "run")) run();
        if (GuiButton((Rectangle){ 50, 180, 200, 30 }, "yajuiku")) yajuiku();
        if (GuiButton((Rectangle){ 50, 220, 200, 30 }, "builrun")) { reset(compilerPath); build(compilerPath); run(); }
        if (GuiButton((Rectangle){ 50, 260, 200, 30 }, "install")) { reset(compilerPath); install(compilerPath); }
        if (GuiButton((Rectangle){ 50, 300, 200, 30 }, "remove")) remove();
        if (GuiButton((Rectangle){ 50, 340, 200, 30 }, "reset")) reset(compilerPath);
        if (GuiButton((Rectangle){ 50, 380, 200, 30 }, "ruun")) ruun();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
