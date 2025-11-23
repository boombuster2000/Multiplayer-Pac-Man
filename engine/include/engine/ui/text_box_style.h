#pragma once

#include "enums.h"
#include "raylib.h"
#include "text_style.h"

namespace ui
{

struct TextBoxStyle
{
    float boarderThickness;
    Color borderColor;
    Color backgroundColor;
    TextStyle backgroundTextStyle;
    TextStyle typingTextStyle;
    Alignment alignment;
    float padding;
};

} // namespace ui