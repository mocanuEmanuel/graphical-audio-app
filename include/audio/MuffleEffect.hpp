#pragma once
#include "IEffect.hpp"
#include <cmath>

namespace synth {

class MuffleEffect : public EffectBase {
public:
    explicit MuffleEffect(float sampleRate = 44100.0f, float cutoffFreq = 1000.0f);
    ~MuffleEffect() override = default;
    
    void applyEffect(std::span<float> audioBuffer) override;
    void reset() override;
    
    void setCutoffFrequency(float frequency);
    [[nodiscard]] float getCutoffFrequency() const;
    void setNormalizedCutoff(float normalized);
    [[nodiscard]] float getSampleRate() const;

private:
    void updateCoefficient();
    
    float m_sampleRate;
    float m_cutoffFreq;
    float m_alpha = 0.0f;
    float m_previousOutput = 0.0f;
    
    static constexpr float MIN_CUTOFF = 20.0f;
    static constexpr float MAX_CUTOFF_RATIO = 0.45f;
};

}