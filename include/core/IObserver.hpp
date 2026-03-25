#pragma once 

#include <any>
#include <string>

namespace synth {
    
    enum class Event{
        TemperatureChanged,
        FrequencyChanged,
        ModulationChanged,
        LightLevelChanged,  
        HumidityChanged, 
        ModeChanged,
        EffectToggled
    };
    //watches for updates
    class IObserver {
    public:
        virtual ~IObserver() = default;
        virtual void onNotify(Event event, const std::any& data) = 0;
    };

    //holds updates
    class ISubject {
    public:
        virtual ~ISubject() = default;
        virtual void addObserver(IObserver* observer) = 0;
        virtual void removeObserver(IObserver* observer) = 0;
        virtual void notifyObserver(Event event, const std::any& data) = 0;
    };
}

