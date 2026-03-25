#include "core/StateManager.hpp"
#include <algorithm>
#include <cmath>

namespace synth {   
    void StateManager::addObserver(IObserver* observer) {
        //safeguard to not update the same observer twice
        if (std::find(m_observers.begin(), m_observers.end(), observer) == m_observers.end()) {
        m_observers.push_back(observer);
        }
    }

    void StateManager::removeObserver(IObserver* observer) {
        m_observers.erase(
            std::remove(m_observers.begin(), m_observers.end(), observer),
            m_observers.end()
        );
    }
    void StateManager::notifyObserver(Event event, const std::any& data) {
        // Copy to avoid issues if observers modify the list by deleting and messing up the memory of the vector
        auto observersCopy = m_observers; 
        for (auto* observer : observersCopy) {
            observer->onNotify(event, data);
        }
    }
    //Getters
    float StateManager::getTemperature() const {
        return m_currentState.temperature;
    }
    float StateManager::getFrequency() const {
        return m_currentState.frequency;
    }
     float StateManager::getModulation() const {
        return m_currentState.modulation;
    }

    bool StateManager::isEffectOn() const {
        return m_currentState.effectOn; 
    }
    int StateManager::getLightLevel() const {
        return m_currentState.lightLevel;
    }
    float StateManager::getHumidity() const {
        return m_currentState.humidity;
    }
    std::string StateManager::getMode() const {
        return m_currentState.mode;
    }

    //Setters
    void StateManager::setTemperature(float temp) {
        m_manuelState.temperature = temp;
        if(m_currentState.mode != "HARDWARE"){
            m_currentState.temperature = temp;
            notifyObserver(Event::TemperatureChanged, temp);
        }
    }
    void StateManager::setFrequency(float freq) {
        m_manuelState.frequency = freq;
        if(m_currentState.mode != "HARDWARE"){
            m_currentState.frequency = freq;
            notifyObserver(Event::FrequencyChanged, freq);
        }
    }
    void StateManager::setModulation(float mod) {
        m_manuelState.modulation = mod;
        if(m_currentState.mode != "HARDWARE"){
            m_currentState.modulation = mod;
            notifyObserver(Event::ModulationChanged, mod);
        }
    }
    void StateManager::toggleEffect(bool on) {
        if(m_currentState.effectOn == on) return; // No change no notify
        m_manuelState.effectOn = on;
        m_currentState.effectOn = on;
        notifyObserver(Event::EffectToggled, on);
    }
    void StateManager::setLightLevel(int level) {
        m_manuelState.lightLevel = level;
        if(m_currentState.mode != "HARDWARE"){
            m_currentState.lightLevel = level;
            notifyObserver(Event::LightLevelChanged, level);
        }
    }
    void StateManager::setHumidity(float humidity) {
        m_manuelState.humidity = humidity;
        if(m_currentState.mode != "HARDWARE"){
            m_currentState.humidity = humidity;
            notifyObserver(Event::HumidityChanged, humidity);
        }
    }
    void StateManager::setMode(const std::string& mode) {
        if(m_currentState.mode == mode) return; // No change no notify
        m_manuelState.mode = mode;
        m_currentState.mode = mode;
        notifyObserver(Event::ModeChanged, mode);

        //if user switches from hardware to manual go back to last slider values
        if (mode == "OFF" || mode == "MANUAL") {
            m_currentState.temperature = m_manuelState.temperature;
            m_currentState.frequency = m_manuelState.frequency;
            m_currentState.effectOn = m_manuelState.effectOn;
            m_currentState.lightLevel = m_manuelState.lightLevel;
            m_currentState.humidity = m_manuelState.humidity;   
        }

        
    }
    void StateManager::update(float deltaTime) {
            //implement interpolation for values coming from hardware and they can be jumpy
            //later
        }
}