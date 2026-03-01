#pragma once

#include "core/IObserver.hpp"
#include "audio/IEffect.hpp"
#include <vector>
#include <memory>
#include <string>
#include <span>
#include "raylib.h"

namespace synth {
    //forward declaration we only referance we avoid circular dependency
    class StateManager; 
    //temporary config for constructor
    struct AudioConfig {
    float sampleRate = 44100.0f;
    int bufferSize = 1024;
    float masterVolume = 0.7f;
};

    class AudioEngine : public IObserver {
    public:
        explicit AudioEngine(StateManager& stateManager, const AudioConfig& config = AudioConfig{});
        ~AudioEngine() override;


        AudioEngine(const AudioEngine&) = delete;
        AudioEngine& operator=(const AudioEngine&) = delete; //prevent copying

        //effect management functions
        void addEffect(std::unique_ptr<IEffect> effect);
        bool removeEffect(const std::string& name);
        [[nodiscard]] const std::vector<std::unique_ptr<IEffect>>& getEffects() const;
        [[nodiscard]] IEffect* getEffect(const std::string& name);

        //audio basic control
        void start();
        void stop();
        bool isPlaying() const;
        void setMasterVolume(float volume);
        [[nodiscard]] float getMasterVolume() const;

        void update();

        void onNotify(Event event, const std::any& data) override;
    private:
        void processAudio();
        void applyEffects(std::span<float> samples);
        void fillAudioStream();
        StateManager& m_stateManager;
        AudioConfig m_config;
        std::vector<std::unique_ptr<IEffect>> m_effects;
        std::vector<float> m_audioBuffer;

        AudioStream m_stream{};

        float m_masterVolume = 0.7f;
        bool m_playing = false;
        bool m_initialized = false;
        bool m_streamInitialized = false;
        //commentig this for now i think i should handle this different
       /* float m_cachedTemperature = 0.0f;
        float m_cachedFrequency = 0.0f;
        bool m_effectOn = false; */
        //... other cached parameters


};
}