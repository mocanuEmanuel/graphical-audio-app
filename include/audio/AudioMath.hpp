#pragma once

#include <cmath>
#include <algorithm>
#include <numbers>

namespace synth::AudioMath {

//One-pole lowpass filter
// input: Current sample x[n]
// state: Previous output y[n-1] (reference)
// alpha: Filter coefficient (0.0 - 1.0)
// Returns: Filtered sample y[n]
// Difference equation: y[n] = (1 - alpha) * x[n] + alpha * y[n-1]
[[nodiscard]] inline constexpr float onePoleLPF(float input, float state, float alpha) noexcept {
    return (1.0f - alpha) * input + alpha * state;
}

//bit crush effect
// input: Input sample (-1.0 to 1.0)
// levels: Number of quantization steps (2^bits - 1)
// ditherNoise: Optional noise value to add before quantization
// Returns: Bit-crushed sample (-1.0 to 1.0)
[[nodiscard]] inline float bitCrush(float input, float levels, float ditherNoise = 0.0f) noexcept {
    // Map [-1, 1] to [0, levels]
    float normalized = (input + 1.0f) * 0.5f;
    float scaled = normalized * levels;
    
    // Add dither and quantize
    float quantized = std::round(scaled + ditherNoise);
    
    // Clamp to valid range
    quantized = (quantized < 0.0f) ? 0.0f : (quantized > levels ? levels : quantized);
    
    // Map back to [-1, 1]
    return (quantized / levels) * 2.0f - 1.0f;
}

//clculate filter coefficient for one-pole LPF
// cutoffFreq: Cutoff frequency in Hz
// sampleRate: Sample rate in Hz
// Returns: Alpha coefficient (0.0 - 1.0)
[[nodiscard]] inline float calculateLPFAlpha(float cutoffFreq, float sampleRate) noexcept {
    return std::exp(-2.0f * std::numbers::pi_v<float> * cutoffFreq / sampleRate);
}

}