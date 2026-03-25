//Waveform generator using pure functional approach
//Mathematical Foundation:
// - Sine wave:     sin(2π * f * t)
// - Square wave:   sign(sin(2π * f * t))
// - Triangle wave: (2/π) * asin(sin(2π * f * t))
// - Sawtooth wave: 2 * (t * f - floor(0.5 + t * f))
//Uses lambdas for stateless waveform generation,
 
#pragma once

#include <functional>
#include <span>
#include <cmath>
#include <cstdint>
#include <numbers>

namespace synth {

//basic waveforms
enum class Waveform {
    Sine,
    Square,
    Triangle,
    Sawtooth,
    Noise
};


//waveform generator function type
 //Current phase (0.0 - 1.0)
//sampleIndex Sample counter for deterministic noise
//returns Sample value (-1.0 to 1.0)
 
using WaveFunction = std::function<float(float phase, uint32_t sampleIndex)>;

//functional math utilities for waveform generation
//- [[nodiscard]] - Return value must be used
//- noexcept
//- no side effects and determinsitc output

namespace MathUtils {
    //linear interpolation
    [[nodiscard]] inline constexpr float lerp(float a, float b, float t) noexcept {
        return a + t * (b - a);
    }
    
   //clamping function
    [[nodiscard]] inline constexpr float clamp(float value, float min, float max) noexcept {
        return value < min ? min : (value > max ? max : value);
    }
    
    //avoiding using rand() because its slower and not functional
    [[nodiscard]] inline constexpr uint32_t xorshift32(uint32_t seed) noexcept {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        return seed;
    }
    
    //convert uint32 random to float in range [-1, 1]
    [[nodiscard]] inline constexpr float uint32ToNormalizedFloat(uint32_t value) noexcept {
        return static_cast<float>(value) / static_cast<float>(UINT32_MAX);
    }
}

//pure functions for waveform generation
//functions are [[nodiscard]], noexcept, and deterministic
namespace WaveFunctions {
    
    // Sine wave: sin(2π * phase)
    // phase Normalized phase [0, 1]
    //returns  Sample value [-1, 1]
    [[nodiscard]] inline float sine(float phase) noexcept {
        return std::sin(2.0f * std::numbers::pi_v<float> * phase);
    }
    
    
     //Square wave: sign(sin(2π * phase))
     //phase Normalized phase [0, 1]
     //returns Sample value {-1, 1}
     
    [[nodiscard]] inline float square(float phase) noexcept {
        return sine(phase) >= 0.0f ? 1.0f : -1.0f;
    }
    
    
    //riangle wave: (2/π) * asin(sin(2π * phase))
   // phase Normalized phase [0, 1]
    //returns  Sample value [-1, 1]
    
    [[nodiscard]] inline float triangle(float phase) noexcept {
        return (2.0f / std::numbers::pi_v<float>) * 
               std::asin(std::sin(2.0f * std::numbers::pi_v<float> * phase));
    }
    
    
    //Sawtooth wave: 2 * (phase - floor(0.5 + phase))
     // phase Normalized phase [0, 1]
    //returns  Sample value [-1, 1]
    [[nodiscard]] inline float sawtooth(float phase) noexcept {
        return 2.0f * (phase - std::floor(0.5f + phase));
    }
    

     // Deterministic pseudo-random noise using xorshift
    // phase Normalized phase [0, 1] - used as seed basis
    // sampleIndex Optional sample index for additional determinism
     //returns Sample value [-1, 1]
     //For truly random-sounding noise, increment sampleIndex each sample.
    
    [[nodiscard]] inline float noise(float phase, uint32_t sampleIndex = 0) noexcept {
        // Combine phase and sample index into a seed
        uint32_t seed = static_cast<uint32_t>(phase * 1000000.0f) ^ (sampleIndex * 2654435761u);
        uint32_t random = MathUtils::xorshift32(seed);
        return MathUtils::uint32ToNormalizedFloat(random) * 2.0f - 1.0f;
    }
}

//maintains phase state and generates samples based on current frequency, amplitude, and waveform
class Oscillator {
public:
    explicit Oscillator(float sampleRate = 44100.0f);
    ~Oscillator() = default;
    
    void setFrequency(float frequency);
    [[nodiscard]] float getFrequency() const;

    void setWaveform(Waveform waveform);
    [[nodiscard]] Waveform getWaveform() const;

    void setAmplitude(float amplitude);
    [[nodiscard]] float getAmplitude() const;
    
    float nextSample();
    void generate(std::span<float> buffer);
    void reset();

private:
    float m_sampleRate;
    float m_frequency = 440.0f;
    float m_phase = 0.0f;
    float m_phaseIncrement = 0.0f;
    float m_amplitude = 0.5f;
    uint32_t m_sampleIndex = 0;  // Sample counter for deterministic noise
    Waveform m_waveform = Waveform::Sine;
    WaveFunction m_waveFunction;
    
    void updatePhaseIncrement();
    void updateWaveFunction();
};

}