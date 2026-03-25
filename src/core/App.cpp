#include "core/App.hpp"
#include "raylib.h"
#include "ui/ThemeManager.hpp"
#include "ui/Slider.hpp"
#include "ui/ShaderWidget.hpp"
#include "audio/AudioEngine.hpp"    

namespace synth {

App::App(int width, int height, const std::string& title) {
    InitWindow(width, height, title.c_str());
    SetTargetFPS(60);
    InitAudioDevice();  

    m_stateManager = std::make_unique<StateManager>();
    m_commandHistory = std::make_unique<CommandHistory>();

    // Create audio engine
    AudioConfig audioConfig;
    audioConfig.sampleRate = 44100.0f;
    audioConfig.bufferSize = 1024;
    audioConfig.masterVolume = 0.7f;
    m_audioEngine = std::make_unique<AudioEngine>(*m_stateManager, audioConfig);

    // starting frequency
    m_stateManager->setFrequency(440.0f);

    //UI setup
    m_uiRoot = std::make_unique<ui::GridContainer>("main_grid", 2, 3);
    m_uiRoot->computeLayout({10.0f, 50.0f, 780.0f, 380.0f});

    //Shaders
    auto waveformShader = std::make_shared<ui::ShaderWidget>("shader_wave", "assets/shaders/waveform.fs");
    waveformShader->setParameter("u_frequency", [this](){ return m_stateManager->getFrequency() / 2000.0f; });
    waveformShader->setParameter("u_amplitude", [this](){ return m_stateManager->getModulation(); });
    waveformShader->setUniformVec3("u_color", 0.0f, 1.0f, 0.8f); 
    waveformShader->setUniformFloat("u_glow", 0.5f);
    waveformShader->setUniformFloat("u_thickness", 2.0f);

    auto electricShader = std::make_shared<ui::ShaderWidget>("shader_elec", "assets/shaders/electric.fs");
    electricShader->setParameter("u_temperature", [this](){ return m_stateManager->getTemperature(); });
    electricShader->setParameter("u_amplitude", [this](){ return m_stateManager->getModulation(); });
    electricShader->setUniformVec3("u_color", 0.8f, 0.2f, 1.0f);
    electricShader->setUniformFloat("u_borderWidth", 0.1f);
    electricShader->setUniformFloat("u_noiseIntensity", 1.0f);

    auto crtShader = std::make_shared<ui::ShaderWidget>("shader_crt", "assets/shaders/crt.fs");
    crtShader->setParameter("scanlineIntensity", [this](){ return m_stateManager->getModulation(); });
    crtShader->setParameter("vignetteStrength", [this](){ return m_stateManager->getTemperature(); });
    crtShader->setUniformFloat("curvature", 0.3f);
    crtShader->setUniformFloat("chromaticAberration", 0.005f);

    m_uiRoot->addWidget(waveformShader, 0, 0);
    m_uiRoot->addWidget(electricShader, 0, 1);
    m_uiRoot->addWidget(crtShader, 0, 2);

    //temp slider
auto tempSlider = std::make_shared<ui::Slider>("temp_slider", 0.0f, 1.0f);
tempSlider->setLabel("TEMP");
tempSlider->setOnValueChanged([this](float val) {
    // 1. Create the command using lambdas
    auto cmd = std::make_unique<StateCommand<float>>(
        "Set Temperature", //command name
        [this]() { return m_stateManager->getTemperature(); }, //getter
        [this](float v) { m_stateManager->setTemperature(v); }, //setter
        val //new val
    );
    // 2. Execute and push to history, passing merge=true!
    m_commandHistory->executeCommand(std::move(cmd), true);
});
tempSlider->setTooltip("Controls temperature parameter");


auto freqSlider = std::make_shared<ui::Slider>("freq_slider", 20.0f, 2000.0f);
freqSlider->setLabel("FREQ");
freqSlider->setOnValueChanged([this](float val) {
    auto cmd = std::make_unique<StateCommand<float>>(
        "Set Frequency",
        [this]() { return m_stateManager->getFrequency(); },
        [this](float v) { m_stateManager->setFrequency(v); },
        val
    );
    m_commandHistory->executeCommand(std::move(cmd), true);
});
freqSlider->setTooltip("Controls oscillator frequency (Hz)");


auto modSlider = std::make_shared<ui::Slider>("mod_slider", 0.0f, 1.0f);
modSlider->setLabel("MOD");
modSlider->setOnValueChanged([this](float val) {
    auto cmd = std::make_unique<StateCommand<float>>(
        "Set Modulation",
        [this]() { return m_stateManager->getModulation(); },
        [this](float v) { m_stateManager->setModulation(v); },
        val
    );
    m_commandHistory->executeCommand(std::move(cmd), true);
});
modSlider->setTooltip("Controls modulation parameter");


    m_uiRoot->addWidget(tempSlider, 1, 0);
    m_uiRoot->addWidget(freqSlider, 1, 1);
    m_uiRoot->addWidget(modSlider, 1, 2);

    m_uiRoot->computeLayout({10.0f, 50.0f, 780.0f, 380.0f});

    // Start audio playback
    m_audioEngine->start();
}

App::~App() {
    m_audioEngine->stop();
    m_uiRoot.reset();
    m_commandHistory.reset();
    m_audioEngine.reset();
    m_stateManager.reset();
    CloseAudioDevice();
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
    //mute/unmute
    if (IsKeyPressed(KEY_SPACE)) {
        if (m_audioEngine->isPlaying()) {
            m_audioEngine->stop();
        } else {
            m_audioEngine->start();
        }
    }
    // Pass mouse inputs to UI
    if (m_uiRoot) {
        m_uiRoot->handleInput();
    }
}

void App::update(float dt) {
    m_stateManager->update(dt);
    m_audioEngine->update();

    if (m_uiRoot) {
        m_uiRoot->update(dt);
    }
}

void App::render() {
    auto& theme = ui::ThemeManager::getIndustrialTheme();

    BeginDrawing();
    ClearBackground(theme.background);

    DrawText("SYNTHERZ app", 20, 20, 20, theme.primary);

    // Audio status indicators
    const char* audioText = m_audioEngine->isPlaying() ? "[AUDIO ON]" : "[MUTED]";
    DrawText(audioText, 200, 20, 14, theme.primary);

    const char* waveNames[] = {"SIN", "SQR", "TRI", "SAW", "NSE"};
    int waveIdx = static_cast<int>(m_audioEngine->getOscillator().getWaveform());
    DrawText(TextFormat("[%s]", waveNames[waveIdx]), 330, 20, 14, theme.primary);

    DrawText("ESC: Quit   (Mouse Only Input)",
             20, 270, 12, theme.primary);

    if (m_uiRoot) {
        m_uiRoot->draw(theme);
    }

    EndDrawing();
}

}