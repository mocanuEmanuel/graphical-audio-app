#pragma once

#include "raylib.h"

namespace synth::ui {


struct Theme {
    Color background;
    Color primary;
    Color secondary;
    Color accent;
    Color text;
    Color textDim;
    Color border;
    int fontSize;
    float borderThickness;
    float padding;
};

//manages themes for the UI
class ThemeManager {
public:
    ThemeManager() = delete;
    
    //grey matte 
    static const Theme& getIndustrialTheme() {
        static Theme theme{
            {15, 15, 18, 255},       // background (dark charcoal)
            {150, 160, 170, 255},    // primary (light metallic gray)
            {100, 110, 120, 255},    // secondary (mid steel gray)
            {220, 100, 50, 255},     // accent (muted industrial orange)
            {200, 210, 220, 255},    // text (off-white/light gray)
            {100, 110, 120, 255},    // textDim (darker gray)
            {40, 45, 50, 255},       // border (dark structural gray)
            16,                      // fontSize
            2.0f,                    // borderThickness
            8.0f                     // padding
        };
        return theme;
    }
};

} 
