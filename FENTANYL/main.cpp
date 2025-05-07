#include "raylib.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    std::filesystem::path basePath = std::filesystem::current_path();
    std::filesystem::path compilerPath = basePath / "compiler";
    std::vector<std::string> args(argv + 1, argv + argc);
    const int screenWidth = 900;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "fentanyL");

    Rectangle buildR = { 80, 230, 190, 60 };
    bool buildcolor = false;
    bool buildtools = false;

    Rectangle runR = { 500, 230, 190, 60 };
    bool runcolor = false;
    bool runsummon = false;

    Rectangle bootstrapR = { 80, 150, 360, 60 };
    bool bootstrapcolor = false;
    bool bootstrapsummon = false;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        Vector2 mousePoint = GetMousePosition();
        if (CheckCollisionPointRec(mousePoint, buildR))
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                buildcolor = false;
                buildtools = true;
            }
            else
            {
                buildcolor = true;
            }
        }
        else buildcolor = false;

        if (CheckCollisionPointRec(mousePoint, runR))
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                runcolor = false;
                runsummon = true;
            }
            else
            {
                runcolor = true;
            }
        }
        else runcolor = false;

        if (CheckCollisionPointRec(mousePoint, bootstrapR))
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                bootstrapcolor = false;
                bootstrapsummon = true;
            }
            else
            {
                bootstrapcolor = true;
            }
        }
        else bootstrapcolor = false;
        
        if (buildtools)
        {
            system("./yajuiku build -n");
            buildtools = false;
        }

        if (runsummon)
        {
            system("./yajuiku run");
            runsummon = false;
        }

        if (bootstrapsummon)
        {
            system("./yajuiku bootstrap");
            bootstrapsummon = false;
        }
        

        BeginDrawing();

        ClearBackground(BLACK);

        DrawRectangle(buildR.x, buildR.y, buildR.width, buildR.height, GRAY);
        DrawRectangle(buildR.x + 5, buildR.y + 5, buildR.width - 10, buildR.height - 10, WHITE);
        if (buildcolor) DrawText("BUILD", buildR.x + 5, buildR.y + 5, 51, GRAY);
        else if (!buildcolor) DrawText("BUILD", buildR.x + 5, buildR.y + 5, 51, BLACK);

        DrawRectangle(runR.x, runR.y, runR.width, runR.height, GRAY);
        DrawRectangle(runR.x + 5, runR.y + 5, runR.width - 10, runR.height - 10, WHITE);
        if (runcolor) DrawText("RUN", runR.x + 5, runR.y + 5, 51, GRAY);
        else if (!runcolor) DrawText("RUN", runR.x + 5, runR.y + 5, 51, BLACK);

        DrawRectangle(bootstrapR.x, bootstrapR.y, bootstrapR.width, bootstrapR.height, GRAY);
        DrawRectangle(bootstrapR.x + 5, bootstrapR.y + 5, bootstrapR.width - 10, bootstrapR.height - 10, WHITE);
        if (bootstrapcolor) DrawText("BOOTSTRAP", bootstrapR.x + 5, bootstrapR.y + 5, 51, GRAY);
        else if (!bootstrapcolor) DrawText("BOOTSTRAP", bootstrapR.x + 5, bootstrapR.y + 5, 51, BLACK);
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
