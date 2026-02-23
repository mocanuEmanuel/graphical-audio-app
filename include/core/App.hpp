#pragma once

#include <string>

namespace synth {

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

};

} 