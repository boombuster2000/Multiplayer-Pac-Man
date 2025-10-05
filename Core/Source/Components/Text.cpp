#include "Text.h"

namespace Components {


    Text::Text(std::string text, TextStyle textStyle, Vector2 anchorPointPosition, AnchorPoint anchorPoint, bool visible)
    : RenderableObject(anchorPointPosition, visible, anchorPoint)
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
    Vector2 Text::GetSize() const
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

    void Text::Render() const
    {
        if (IsVisible())
        {
            Vector2 position = GetPosition();
            DrawText(m_text.c_str(), static_cast<int>(position.x), static_cast<int>(position.y), m_style.fontSize, m_style.color);
        }
    }

}