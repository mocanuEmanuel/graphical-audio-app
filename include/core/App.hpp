#pragma once

#include <string>
#include "ui/GridContainer.hpp"
#include "core/StateManager.hpp"
#include "core/CommandHistory.hpp"

namespace synth {

class AudioEngine;
class MuffleEffect;

class App {
public:
    App(int width, int height, const std::string& title);
    ~App();

    App(const App&) = delete;
    App& operator=(const App&) = delete;

    int run();

private:
    bool m_running = true;

    void handleInput();      
    void update(float dt);    
    void render();  

    std::unique_ptr<StateManager> m_stateManager;
    std::unique_ptr<AudioEngine> m_audioEngine;
    std::unique_ptr<CommandHistory> m_commandHistory;
    std::unique_ptr<ui::GridContainer> m_uiRoot;

};

} 