#include "engine/ui/text.h"

namespace ui
{

Text::Text(std::string text, TextStyle textStyle, Vector2Ex<float> anchorPointPosition, AnchorPoint anchorPoint,
           bool visible)
    : RenderableObject(anchorPointPosition, anchorPoint, visible)
{
    SetText(text);
    SetFontSize(textStyle.fontSize);
    SetColor(textStyle.color);
    SetOrigin(anchorPoint);
}

void Text::SetText(std::string text)
{
    m_text = text;
}

std::string Text::GetText() const
{
    return m_text;
}

void Text::SetFontSize(float fontSize)
{
    m_style.fontSize = fontSize;
}

float Text::GetFontSize() const
{
    return m_style.fontSize;
}

void Text::SetColor(Color color)
{
    m_style.color = color;
}

Color Text::GetColor() const
{
    return m_style.color;
}
Vector2Ex<float> Text::GetDimensions() const
{
    return MeasureTextEx(GetFontDefault(), m_text.c_str(), m_style.fontSize, 1.0f);
}

void Text::SetStyle(TextStyle style)
{
    m_style = style;
}

TextStyle Text::GetStyle() const
{
    return m_style;
}

void Text::Render(Vector2Ex<float> offset) const
{
    if (IsVisible())
    {
        const Vector2Ex<float> position = GetWorldOrigin() + offset;
        DrawTextPro(GetFontDefault(), m_text.c_str(), position, GetObjectOrigin(), 0.0f, m_style.fontSize, 1.0f,
                    m_style.color);
    }
}

} // namespace ui