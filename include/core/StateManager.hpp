#pragma once
#include "IObserver.hpp"
#include <vector>
#include <algorithm>

namespace synth {

 struct SynthState{
    float temperature = 0.0f;
    float frequency = 0.0f;
    float modulation = 0.0f;
    bool effectOn = false;
    int lightLevel = 0;
    float humidity = 0.0f;
    std::string mode = "OFF";
 };
 
class StateManager : public ISubject {
public:
    StateManager() = default;
    ~StateManager() override = default;
    //needs a single instance
    StateManager(const StateManager&) = delete;

    void addObserver(IObserver* observer) override;
    void removeObserver(IObserver* observer) override;
    
    //Getters
    [[nodiscard]] float getTemperature() const;
    [[nodiscard]] float getFrequency() const;
    [[nodiscard]] float getModulation() const;
    [[nodiscard]] bool isEffectOn() const;
    [[nodiscard]] int getLightLevel() const;
    [[nodiscard]] float getHumidity() const;
    [[nodiscard]] std::string getMode() const;

    //Setters
    void setTemperature(float temp);
    void setFrequency(float freq);
    void setModulation(float mod);
    void toggleEffect(bool on);
    void setLightLevel(int level);
    void setHumidity(float humidity);   
    void setMode(const std::string& mode);

    void update(float deltaTime);

    private:
        void notifyObserver(Event event, const std::any& data) override;
        SynthState m_currentState;
        SynthState m_hardwareState;
        SynthState m_manuelState;

        std::vector<IObserver*> m_observers;
    
        // Adjust for faster/slower transitions
        static constexpr float INTERPOLATION_SPEED = 5.0f; 

};
}