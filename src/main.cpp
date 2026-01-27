#include "raylib.h"

int main()
{
    InitWindow(800, 450, "SYNTH_ERR [STARTUP]");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground({15, 15, 15, 255}); // Terminal Dark
        DrawText("SYSTEM_BOOT: SUCCESS", 20, 20, 20, LIME);
        DrawText("> C++20 ACTIVE", 20, 50, 20, RAYWHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}