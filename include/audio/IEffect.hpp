//Strategy pattern for applying the different audio effects

#pragma once

#include <span>

namespace synth {

    class IEffect {
    public:
        virtual ~IEffect() = default;
        //audioBuffer is pointer to audio data
        //audioFrames is the number of frames in the buffer
        virtual void applyEffect(std::span<float> audioBuffer) = 0;

    };


}