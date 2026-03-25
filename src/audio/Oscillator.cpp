#include "audio/Oscillator.hpp"
#include <algorithm>

namespace synth {

Oscillator::Oscillator(float sampleRate)
    : m_sampleRate(sampleRate)
{
    updatePhaseIncrement();
    updateWaveFunction();
}

void Oscillator::setFrequency(float frequency) {
    m_frequency = frequency;
    updatePhaseIncrement();
}

float Oscillator::getFrequency() const {
    return m_frequency;
}

void Oscillator::setWaveform(Waveform waveform) {
    m_waveform = waveform;
    updateWaveFunction();
}

Waveform Oscillator::getWaveform() const {
    return m_waveform;
}

void Oscillator::setAmplitude(float amplitude) {
    m_amplitude = std::clamp(amplitude, 0.0f, 1.0f);
}

float Oscillator::getAmplitude() const {
    return m_amplitude;
}

float Oscillator::nextSample() {
    float sample = m_waveFunction(m_phase, m_sampleIndex) * m_amplitude;
    
    // Advance phase and sample counter
    m_phase += m_phaseIncrement;
    if (m_phase >= 1.0f) {
        m_phase -= 1.0f;
    }
    ++m_sampleIndex;
    
    return sample;
}

void Oscillator::generate(std::span<float> buffer) {
    for (auto& sample : buffer) {
        sample = nextSample();
    }
}

void Oscillator::reset() {
    m_phase = 0.0f;
    m_sampleIndex = 0;
}

void Oscillator::updatePhaseIncrement() {
    // Phase increment per sample = frequency / sampleRate
    m_phaseIncrement = m_frequency / m_sampleRate;
}

void Oscillator::updateWaveFunction() {
    // Wrap pure functions with consistent signature including sample index
    switch (m_waveform) {
        case Waveform::Sine:
            m_waveFunction = [](float phase, uint32_t /*SampleIndex*/) { 
                return WaveFunctions::sine(phase); 
            };
            break;
        case Waveform::Square:
            m_waveFunction = [](float phase, uint32_t /*SampleIndex*/) { 
                return WaveFunctions::square(phase); 
            };
            break;
        case Waveform::Triangle:
            m_waveFunction = [](float phase, uint32_t /*SampleIndex*/) { 
                return WaveFunctions::triangle(phase); 
            };
            break;
        case Waveform::Sawtooth:
            m_waveFunction = [](float phase, uint32_t /*SampleIndex*/) { 
                return WaveFunctions::sawtooth(phase); 
            };
            break;
        case Waveform::Noise:
            m_waveFunction = [](float phase, uint32_t /*SampleIndex*/) { 
                return WaveFunctions::noise(phase, 0); 
            };
            break;
    }
}

} 