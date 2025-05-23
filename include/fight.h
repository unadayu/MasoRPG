#include <SDL2/SDL.h>
#include <filesystem>
#include <vector>
#include "function.h" // drawText や DrawRectangleLines を含む

class fightUI {
public:
    void runGameGame(SDL_Renderer* renderer, Rectangle WindowSize, const std::filesystem::path& fontPath) {
        this->fontPath = fontPath;
        PlayerFlame(renderer, WindowSize, true, true);
    }

private:
    InGamePlayerData playerRect = {0, 0, 9315, 9315};
    std::filesystem::path fontPath;

    void flame(SDL_Renderer* renderer, Rectangle WindowSize) {
        const int boxX = 30;
        const int boxWidth = 150;
        const int boxHeight = 60;
        const int fontSize = 24;
        SDL_Color borderColor = {255, 255, 255, 255};

        struct Command {
            const char* label;
            int y;
        };

        std::vector<Command> commands = {
            {"バトル", 50},
            {"スキル", 140},
            {"チャージ", 230},
            {"逃げる", 320}
        };

        for (const auto& cmd : commands) {
            DrawRectangleLines(boxX, cmd.y, boxWidth, boxHeight, borderColor, renderer);
            drawText(renderer, 255, 255, 255, fontPath, fontSize, cmd.label, boxX + 10, cmd.y + 15);
        }
    }

    void PlayerFlame(SDL_Renderer* renderer, Rectangle WindowSize, bool playerTurn, bool flameRect) {
        SDL_Color borderColor = {255, 255, 255, 255};
        if (playerTurn) {
            if (flameRect) {
                DrawRectangleLines(WindowSize.Width / 2 - 110, WindowSize.Height - 230, 200, 200, borderColor, renderer);
            } else {
                DrawRectangleLines(WindowSize.Width / 2 - 380, WindowSize.Height - 230, WindowSize.Width - 50, 200, borderColor, renderer);
            }
        } else {
            DrawRectangleLines(WindowSize.Width / 2 - 200, WindowSize.Height - 210, WindowSize.Width - 300, 200, borderColor, renderer);
            flame(renderer, WindowSize);
        }
    }
};
