#include "audio/MuffleEffect.hpp"
#include "audio/AudioMath.hpp"
#include <algorithm>
#include <cmath>

namespace synth {

MuffleEffect::MuffleEffect(float sampleRate, float cutoffFreq)
    : EffectBase("Muffle"), m_sampleRate(sampleRate), m_cutoffFreq(cutoffFreq)
{
    updateCoefficient();
}

void MuffleEffect::applyEffect(std::span<float> audioBuffer) {
    if (!m_enabled) return;
    for (auto& sample : audioBuffer) {
        float output = AudioMath::onePoleLPF(sample, m_previousOutput, m_alpha);
        m_previousOutput = output;
        sample = output;
    }
}

void MuffleEffect::reset() {
    m_previousOutput = 0.0f;
}

void MuffleEffect::setCutoffFrequency(float frequency) {
    float maxCutoff = m_sampleRate * MAX_CUTOFF_RATIO;
    m_cutoffFreq = std::clamp(frequency, MIN_CUTOFF, maxCutoff);
    updateCoefficient();
}

float MuffleEffect::getCutoffFrequency() const {
    return m_cutoffFreq;
}

void MuffleEffect::setNormalizedCutoff(float normalized) {
    // Map 0.0-1.0 to exponential frequency range
    normalized = std::clamp(normalized, 0.0f, 1.0f);
    
    float maxCutoff = m_sampleRate * MAX_CUTOFF_RATIO;
    // Exponential mapping for perceptually linear control
    float freq = MIN_CUTOFF * std::pow(maxCutoff / MIN_CUTOFF, normalized);
    setCutoffFrequency(freq);
}

float MuffleEffect::getSampleRate() const {
    return m_sampleRate;
}

void MuffleEffect::updateCoefficient() {
    m_alpha = AudioMath::calculateLPFAlpha(m_cutoffFreq, m_sampleRate);
}

}