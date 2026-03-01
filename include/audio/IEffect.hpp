//Strategy pattern for applying the different audio effects

#pragma once

#include <span>
#include <string>
#include <memory>

namespace synth {

    class IEffect {
    public:
        virtual ~IEffect() = default;
        //audioBuffer is pointer to audio data
        
        virtual void applyEffect(std::span<float> audioBuffer) = 0;

        [[nodiscard]] virtual std::string getName() const = 0;
        [[nodiscard]] virtual bool isEnabled() const = 0;
        virtual void setEnabled(bool enabled) = 0;
        virtual void reset() = 0;
    };

    class EffectBase : public IEffect {
    public:
        explicit EffectBase(std::string name);
        ~EffectBase() override = default;
        
        [[nodiscard]] std::string getName() const override;
        [[nodiscard]] bool isEnabled() const override;
        void setEnabled(bool enabled) override;
        void reset() override;

    protected:
        std::string m_name;
        bool m_enabled = true;
    };
    //factory function type for creating effects
    using EffectFactory = std::unique_ptr<IEffect>(*)();

    }


