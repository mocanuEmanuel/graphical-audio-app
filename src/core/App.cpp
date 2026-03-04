#include "core/App.hpp"
#include "raylib.h"
#include "ui/ThemeManager.hpp"
#include "ui/Slider.hpp"

namespace synth {

App::App(int width, int height, const std::string& title) {
    InitWindow(width, height, title.c_str());
    SetTargetFPS(60);

    m_stateManager = std::make_unique<StateManager>();

    m_uiRoot = std::make_unique<ui::GridContainer>("main_grid", 1, 3);
    
    m_uiRoot->computeLayout({10.0f, 300.0f, 780.0f, 140.0f});

    //temp slider(mock)
    auto tempSlider = std::make_shared<ui::Slider>("temp_slider", 0.0f, 1.0f);
    tempSlider->setLabel("TEMP");
    tempSlider->setValue(m_stateManager->getTemperature());
    tempSlider->setOnValueChanged([this](float val) { m_stateManager->setTemperature(val); });
    m_uiRoot->addWidget(tempSlider, 0, 0);

    //freq mock
    auto freqSlider = std::make_shared<ui::Slider>("freq_slider", 20.0f, 2000.0f);
    freqSlider->setLabel("FREQ");
    freqSlider->setValue(440.0f);
    freqSlider->setOnValueChanged([this](float val) { m_stateManager->setFrequency(val); });
    m_stateManager->setFrequency(440.0f);
    m_uiRoot->addWidget(freqSlider, 0, 1);

    //modulation mock
    auto modSlider = std::make_shared<ui::Slider>("mod_slider", 0.0f, 1.0f);
    modSlider->setLabel("MOD");
    modSlider->setOnValueChanged([this](float val) { /* stateManager update */ });
    m_uiRoot->addWidget(modSlider, 0, 2);

    //layout recomputation after adding sliders
    m_uiRoot->computeLayout({10.0f, 300.0f, 780.0f, 140.0f});
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
    
    // Pass mouse inputs to UI
    if (m_uiRoot) {
        m_uiRoot->handleInput();
    }
}

void App::update(float dt) {
    if (m_uiRoot) {
        m_uiRoot->update(dt);
    }
}

void App::render() {
    //setting theme only one for now
    auto& theme = ui::ThemeManager::getIndustrialTheme();

    BeginDrawing();
    ClearBackground(theme.background);
    
    //header
    DrawText("SYNTHERZ app", 20, 20, 20, theme.primary);

    //render grid layout
    if (m_uiRoot) {
        m_uiRoot->draw(theme);
    }

    EndDrawing();
}

}