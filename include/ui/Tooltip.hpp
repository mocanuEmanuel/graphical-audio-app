#pragma once
#include "raylib.h"
#include "ThemeManager.hpp"
#include <string>

namespace synth::ui {

inline void DrawTooltip(const std::string& text, Vector2 position, const Theme& theme) {
    int fontSize = theme.fontSize - 2;
    int padding = 6;

    int textWidth = MeasureText(text.c_str(), fontSize);
    int textHeight = fontSize;

    Rectangle box = {
        position.x + 12,
        position.y + 12,
        (float)(textWidth + padding * 2),
        (float)(textHeight + padding * 2)
    };

    DrawRectangleRec(box, Fade(theme.background, 0.95f));
    DrawRectangleLinesEx(box, 1.0f, theme.border);

    DrawText(text.c_str(),
             (int)(box.x + padding),
             (int)(box.y + padding),
             fontSize,
             theme.text);
}

}