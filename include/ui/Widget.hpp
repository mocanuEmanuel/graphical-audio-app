#pragma once
#include <string>
#include "raylib.h"
#include "ThemeManager.hpp"

namespace synth::ui {
//base class for all UI elements
class Widget {
public:
    explicit Widget(std::string id) : m_id(std::move(id)) {}
    virtual ~Widget() = default;
    //designating virtuals since raylib has global coordinates
    //so we dont need to do checks on coordinates for all clicks
    //but its separated to each widget to handle its own input and drawing
    virtual void update(float deltaTime) {}
    virtual void draw(const Theme& theme) const = 0;
    
    //returns true if the widget consumed the input 
    virtual bool handleInput() { return false; }
    
    //called by parent to assign spatial bounds
    virtual void computeLayout(Rectangle availableSpace) {
        m_bounds = availableSpace;
    }
    void setTooltip(const std::string& text) { m_tooltip = text; }


protected:
    std::string m_id;
    Rectangle m_bounds{0, 0, 0, 0};
    bool m_focused = false;
    bool m_hovered = false;
    bool m_visible = true;
    bool m_enabled = true;
    std::string m_tooltip;

};

}