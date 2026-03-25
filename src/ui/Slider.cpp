#include "ui/Slider.hpp"
#include <algorithm>
#include "raymath.h"
#include "ui/Tooltip.hpp"

namespace synth::ui {

void Slider::setValue(float val) {
    m_value = std::clamp(val, m_min, m_max);
    if (m_onValueChanged) {
        m_onValueChanged(m_value);
    }
}

void Slider::draw(const Theme& theme) const {
    if (!m_visible) return;

    Vector2 mousePos = GetMousePosition();
    
    float padding = theme.padding;
    float startX = m_bounds.x + padding;
    float y = m_bounds.y + padding;
    
    std::string fullLabel = m_label + ":";
    DrawText(fullLabel.c_str(), static_cast<int>(startX), static_cast<int>(y), theme.fontSize, theme.textDim);

    // tooltip icon
    float questionX = startX + 60.0f;
    float questionY = y;

    Rectangle helpRect = { questionX, questionY, 14.0f, 14.0f };
    DrawText("?", (int)questionX, (int)questionY, theme.fontSize, theme.accent);

    
    
    float barX = startX + 80.0f; 
    float barWidth = m_bounds.width - padding * 2.0f - 160.0f;
    if (barWidth < 50.0f) barWidth = 50.0f;
    
    float progress = getNormalizedValue();
    
    Rectangle barBg = { barX, y + 4.0f, barWidth, (float)theme.fontSize - 8.0f };
    Rectangle barFg = { barX, y + 4.0f, barWidth * progress, (float)theme.fontSize - 8.0f };
    
    DrawRectangleLinesEx(barBg, theme.borderThickness, theme.border);
    
    bool isHoveredLocally = CheckCollisionPointRec(mousePos, barBg);
    Color barColor = m_enabled 
        ? (isHoveredLocally || m_isDragging ? theme.accent : theme.primary)
        : theme.textDim;
        
    DrawRectangleRec(barFg, barColor);
    
    const char* valStr = TextFormat("%.2f", m_value);
    float valueX = barX + barWidth + 10.0f;
    DrawText(valStr, static_cast<int>(valueX), static_cast<int>(y), theme.fontSize, theme.text);

        bool helpHovered = CheckCollisionPointRec(mousePos, helpRect);

    if (helpHovered && !m_tooltip.empty()) {
        DrawTooltip(m_tooltip, mousePos, theme);
    }

}

bool Slider::handleInput() {
    if (!m_visible || !m_enabled) return false;
    
    Vector2 mousePos = GetMousePosition();
    float padding = 8.0f; 
    float barX = m_bounds.x + padding + 80.0f;
    float barWidth = m_bounds.width - padding * 2.0f - 160.0f;
    if (barWidth < 50.0f) barWidth = 50.0f;
    
    Rectangle barBounds = {barX, m_bounds.y, barWidth, m_bounds.height};
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePos, barBounds)) {
            m_isDragging = true;
            m_focused = true; 
        }
    }
    
    if (m_isDragging) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            float normalized = Clamp((mousePos.x - barX) / barWidth, 0.0f, 1.0f);
            float newValue = m_min + normalized * (m_max - m_min);
            setValue(newValue);
            return true;
        } else {
            m_isDragging = false;
        }
    }
    return m_isDragging;
}

}