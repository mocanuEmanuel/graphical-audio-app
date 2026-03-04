#pragma once
#include "Widget.hpp"
#include <functional>

namespace synth::ui {

class Slider : public Widget {
public:
    Slider(std::string id, float min, float max)
        : Widget(std::move(id)), m_min(min), m_max(max), m_value(min) {}

    void setLabel(std::string label) { m_label = std::move(label); }
    void setValue(float val);
    
    //callback when dragging mouse
    void setOnValueChanged(std::function<void(float)> cb) { m_onValueChanged = std::move(cb); }

    void draw(const Theme& theme) const override;
    //CheckCollisionPointRec(mousePos, m_bounds) from raylib directly
    bool handleInput() override; 

private:
    float getNormalizedValue() const { return (m_value - m_min) / (m_max - m_min); }

    std::string m_label;
    float m_value;
    float m_min;
    float m_max;
    bool m_isDragging = false;
    
    std::function<void(float)> m_onValueChanged;
};
}