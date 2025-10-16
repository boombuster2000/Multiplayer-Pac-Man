#include "Text.h"

namespace UIComponents
{

    Text::Text(std::string text, TextStyle textStyle, Vector2Ex<int> anchorPointPosition, AnchorPoint anchorPoint, bool visible)
        : RenderableObject(anchorPointPosition, anchorPoint, visible)
    {
        SetText(text);
        SetFontSize(textStyle.fontSize);
        SetColor(textStyle.color);
        UpdateDrawPoint();
    }

    void Text::SetText(std::string text)
    {
        m_text = text;
        UpdateDrawPoint();
    }

    std::string Text::GetText() const
    {
        return m_text;
    }

    void Text::SetFontSize(int fontSize)
    {
        m_style.fontSize = fontSize;
        UpdateDrawPoint();
    }

    int Text::GetFontSize() const
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
    Vector2Ex<int> Text::GetSize() const
    {
        return MeasureTextEx(GetFontDefault(), m_text.c_str(), static_cast<float>(m_style.fontSize), 1.0f);
    }

    void Text::SetStyle(TextStyle style)
    {
        m_style = style;
        UpdateDrawPoint();
    }

    TextStyle Text::GetStyle() const
    {
        return m_style;
    }

    void Text::Render(Vector2Ex<int> offset) const
    {
        if (IsVisible())
        {
            const Vector2Ex<int> &position = GetPosition();
            DrawTextEx(GetFontDefault(), m_text.c_str(), position + offset, m_style.fontSize, 1, m_style.color);

            // DrawText(m_text.c_str(), static_cast<int>(position.x) + offset.x, static_cast<int>(position.y) + offset.y, m_style.fontSize, m_style.color);
        }
    }

}