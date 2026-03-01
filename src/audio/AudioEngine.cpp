#include "audio/AudioEngine.hpp"
#include "core/StateManager.hpp"
#include <algorithm>

namespace synth {

    EffectBase::EffectBase(std::string name)
    : m_name(std::move(name)) {}

    std::string EffectBase::getName() const { return m_name; }
    bool EffectBase::isEnabled() const { return m_enabled; }
    void EffectBase::setEnabled(bool enabled) { m_enabled = enabled; }
    void EffectBase::reset() {}

    AudioEngine::AudioEngine(StateManager& stateManager, const AudioConfig& config)
        : m_stateManager(stateManager), m_config(config) {
        m_stateManager.addObserver(this);
        m_audioBuffer.resize(m_config.bufferSize);
    }

    AudioEngine::~AudioEngine() {
        stop();
        if (m_streamInitialized) {
            UnloadAudioStream(m_stream);
        }
        m_stateManager.removeObserver(this);
    }

    void AudioEngine::addEffect(std::unique_ptr<IEffect> effect) {
        m_effects.push_back(std::move(effect));
    }

    bool AudioEngine::removeEffect(const std::string& name) {
        auto it = std::remove_if(m_effects.begin(), m_effects.end(),
            [&name](const std::unique_ptr<IEffect>& effect) {
                return effect->getName() == name;
            });
        if (it != m_effects.end()) {
            m_effects.erase(it, m_effects.end());
            return true;
        }
        return false;
    }

    const std::vector<std::unique_ptr<IEffect>>& AudioEngine::getEffects() const {
        return m_effects;
    }

    IEffect* AudioEngine::getEffect(const std::string& name) {
        for (const auto& effect : m_effects) {
            if (effect->getName() == name) {
                return effect.get();
            }
        }
        return nullptr;
    }
    void AudioEngine::setMasterVolume(float volume) {
    m_masterVolume = std::clamp(volume, 0.0f, 1.0f);
    if (m_streamInitialized) {
        SetAudioStreamVolume(m_stream, m_masterVolume);
    }
}

float AudioEngine::getMasterVolume() const {
    return m_masterVolume;
}

void AudioEngine::start() {
    if (m_streamInitialized && !m_playing && m_initialized) {
        PlayAudioStream(m_stream);
        m_playing = true;
    }
}

void AudioEngine::stop() {
    if (m_streamInitialized && m_playing) {
        StopAudioStream(m_stream);
        m_playing = false;
    }
}

bool AudioEngine::isPlaying() const {
    return m_playing;
}

void AudioEngine::update() {
    if (!m_playing || !m_initialized) return;
    
    fillAudioStream();
}

void AudioEngine::fillAudioStream() {
    if (!m_streamInitialized) return;
    
    // check if the hardware buffer needs more chunks
    if (IsAudioStreamProcessed(m_stream)) {
        processAudio();
        
        // push processed chunks to raylib
        UpdateAudioStream(m_stream, m_audioBuffer.data(),static_cast<int>(m_audioBuffer.size()));
    }
}

void AudioEngine::processAudio() {
   //need to implement oscillator later for now silence
    std::fill(m_audioBuffer.begin(), m_audioBuffer.end(), 0.0f);
    
    applyEffects(m_audioBuffer);
}

void AudioEngine::applyEffects(std::span<float> audioBuffer) {
    for (auto& effect : m_effects) {
        if (effect && effect->isEnabled()) {
            effect->applyEffect(audioBuffer);
        }
    }
}

void AudioEngine::onNotify(Event event, const std::any& data) {
    //later
    (void)event;
    (void)data;
}

}