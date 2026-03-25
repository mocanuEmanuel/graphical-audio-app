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
    : m_stateManager(stateManager), m_config(config), m_oscillator(config.sampleRate)
{
    m_stateManager.addObserver(this);
    m_audioBuffer.resize(m_config.bufferSize);
    m_oscillator.setFrequency(m_stateManager.getFrequency());

    //audio stream initialization
    SetAudioStreamBufferSizeDefault(m_config.bufferSize);
    m_stream = LoadAudioStream(
        static_cast<unsigned int>(m_config.sampleRate),
        32,   //32bit float samples
        1     //mono channel
    );
    
    if (IsAudioStreamValid(m_stream)) {
        m_streamInitialized = true;
        SetAudioStreamVolume(m_stream, m_masterVolume);
    }
    
    m_initialized = true;
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

    Oscillator& AudioEngine::getOscillator() {
        return m_oscillator;
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
    if (m_streamInitialized && m_initialized) {
        m_isPlaying = true;
        // Prime the buffer before playing
        fillAudioStream();
        
        if (!IsAudioStreamPlaying(m_stream)) {
            PlayAudioStream(m_stream);
        } else {
            ResumeAudioStream(m_stream);
        }
    }
}


void AudioEngine::stop() {
    if (m_streamInitialized) {
        m_isPlaying = false;
        // pause op to maintain state
        PauseAudioStream(m_stream);
    }
}

bool AudioEngine::isPlaying() const {
        return m_isPlaying;
}

void AudioEngine::update() {
    if (!m_initialized || !isPlaying()) return;
    
    // In case raylib starved and fully stopped the stream internally, resume it
    if (!IsAudioStreamPlaying(m_stream)) {
        ResumeAudioStream(m_stream);
        if (!IsAudioStreamPlaying(m_stream)) {
            PlayAudioStream(m_stream);
        }
    }
    
    fillAudioStream();
}

void AudioEngine::fillAudioStream() {
    if (!m_streamInitialized) return;
    
    // check if the hardware buffer needs more chunks
    while (IsAudioStreamProcessed(m_stream)) {
        processAudio();
        
        // push processed chunks to raylib
        UpdateAudioStream(m_stream, m_audioBuffer.data(),static_cast<int>(m_audioBuffer.size()));
    }
}

void AudioEngine::processAudio() {
    m_oscillator.generate(m_audioBuffer);
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
    switch (event) {
        case Event::FrequencyChanged:
            if (data.has_value()) {
                try {
                    float freq = std::any_cast<float>(data);
                    m_oscillator.setFrequency(freq);
                } catch (const std::bad_any_cast&) {}
            }
            break;
            
        case Event::ModulationChanged:
            if (data.has_value()) {
                try {
                    float mod = std::any_cast<float>(data);
                    // Modulate oscillator amplitude: base 0.3 + mod scales up to 0.7
                    m_oscillator.setAmplitude(0.3f + mod * 0.4f);
                } catch (const std::bad_any_cast&) {}
            }
            break;
            
        case Event::TemperatureChanged:
            // Temperature → Muffle effect cutoff
            // Handled per-frame in App::update() to keep AudioEngine generic
            break;
            
        case Event::EffectToggled:
            // Toggle all effects on/off (global switch)
            if (data.has_value()) {
                try {
                    bool on = std::any_cast<bool>(data);
                    for (auto& effect : m_effects) {
                        if (effect) effect->setEnabled(on);
                    }
                } catch (const std::bad_any_cast&) {}
            }
            break;
            
        default:
            break;
    }
}

}