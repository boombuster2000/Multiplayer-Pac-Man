#include "Text.h"

namespace UIComponents
{

    Text::Text(std::string text, TextStyle textStyle, Vector2Ex<int> anchorPointPosition, AnchorPoint anchorPoint, bool visible)
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

    void Text::SetFontSize(int fontSize)
    {
        m_style.fontSize = fontSize;
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
    Vector2Ex<int> Text::GetDimensions() const
    {
        return MeasureTextEx(GetFontDefault(), m_text.c_str(), static_cast<float>(m_style.fontSize), 1.0f);
    }

    void Text::SetStyle(TextStyle style)
    {
        m_style = style;
    }

    TextStyle Text::GetStyle() const
    {
        return m_style;
    }

    void Text::Render(Vector2Ex<int> offset) const
    {
        if (IsVisible())
        {
            const Vector2Ex<int> &position = GetWorldOrigin();
            DrawTextPro(GetFontDefault(),
                        m_text.c_str(),
                        Vector2{static_cast<float>(position.x + offset.x), static_cast<float>(position.y + offset.y)},
                        GetObjectOrigin(),
                        0.0f,
                        static_cast<float>(m_style.fontSize),
                        1.0f,
                        m_style.color);
        }
    }

}