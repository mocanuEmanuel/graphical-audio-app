#pragma once

#include "core/IObserver.hpp"
#include "audio/IEffect.hpp"
#include <vector>
#include <memory>
#include <string>

namespace synth {

    class AudioEngine : public IObserver {
    public:
        AudioEngine();
        ~AudioEngine() override;

        AudioEngine(const AudioEngine&) = delete;
        AudioEngine& operator=(const AudioEngine&) = delete; //prevent copying

        void addEffect(std::unique_ptr<IEffect> effect);
        void onNotify(Event event, const std::any& data) override;
    private:
        std::vector<std::unique_ptr<IEffect>> m_effects;

        float m_cachedTemperature = 0.0f;
        float m_cachedFrequency = 0.0f;
        bool m_effectOn = false;
        //... other cached parameters


};
}