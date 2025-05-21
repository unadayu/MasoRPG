#include "raylib.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <thread>
#include <chrono>
#include "function.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    std::filesystem::path basePath = std::filesystem::current_path();
    std::filesystem::path compilerPath = basePath / "compiler";
    std::vector<std::string> args(argv + 1, argv + argc);
    const int screenWidth = 900;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "fentanyL");

    buttonHayashi builR = { 80, 230, 190, 60, false, false};
    // Rectangle buildR = { 80, 230, 190, 60 };
    // bool buildcolor = false;
    // bool buildtools = false;

    buttonHayashi runR = { 500, 230, 190, 60, false, false};
    // Rectangle runR = { 500, 230, 190, 60 };
    // bool runcolor = false;
    // bool runsummon = false;

    buttonHayashi bootstrapR = { 80, 150, 360, 60, false, false};
    // Rectangle bootstrapR = { 80, 150, 360, 60 };
    // bool bootstrapcolor = false;
    // bool bootstrapsummon = false;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        Vector2 mousePoint = GetMousePosition();
        if (CheckCollisionPointRec(mousePoint, buildR))
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                buildR.color = false;
                buildR.tool = true;
            }
            else
            {
                buildR.color = true;
            }
        }
        else buildR.color = false;

        if (CheckCollisionPointRec(mousePoint, runR))
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                runR.color = false;
                runR.tool = true;
            }
            else
            {
                runR.color = true;
            }
        }
        else runR.color = false;

        if (CheckCollisionPointRec(mousePoint, bootstrapR))
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                bootstrapR.color = false;
                bootstrapR.tool = true;
            }
            else
            {
                bootstrapR.color = true;
            }
        }
        else bootstrapR.color = false;
        
        if (buildR.tool)
        {
            system("./yajuiku fentanyLbuild");
            buildR.tool = false;
        }

        if (runR.tool)
        {
            system("./yajuiku fentanyLrun");
            runR.summon = false;
        }

        if (bootstrapR.tool)
        {
            system("./yajuiku bootstrap");
            bootstrapR.tool = false;
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
