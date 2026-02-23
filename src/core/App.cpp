#include "core/App.hpp"
#include "raylib.h"

namespace synth {

App::App(int width, int height, const std::string& title) {
    InitWindow(width, height, title.c_str());
    SetTargetFPS(60);
}

App::~App() {
    CloseWindow();
}

int App::run() {
    while (m_running && !WindowShouldClose()) {
        float dt = GetFrameTime();
        handleInput();
        update(dt);
        render();
    }
    return 0;
}

void App::handleInput() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        m_running = false;
    }
}

void App::update(float dt) {
    
}

void App::render() {
    BeginDrawing();
    ClearBackground({15, 15, 15, 255});
    DrawText("SYSTEM_BOOT: SUCCESS", 20, 20, 20, GREEN);
    DrawText("> C++20 ACTIVE", 20, 50, 20, RAYWHITE);
    EndDrawing();
}

} 