#pragma once
#include "DataTypes/Vector2Ex.h"
#include "RenderableObject.h"
#include "Text.h"
#include <string>

namespace UIComponents
{
class TextBox : public RenderableObject
{

  private:
    Vector2Ex<float> m_dimensions;
    Text m_text;
    TextStyle m_typedTextStyle;
    Alignment m_textAlignment;
    float m_padding;
    TextStyle m_backgroundTextStyle;
    Text m_backgroundText;

    bool m_cursorVisible;
    float m_cursorTimer;

    void HandleKeyPresses();
    void UpdateCursor();
    void UpdateTextPosition();
    Vector2Ex<float> CalculateTextPositionOffset(const Vector2Ex<float> &boxDimensions,
                                                 const Vector2Ex<float> &textSize) const;

  public:
    TextBox(Vector2Ex<float> position, Vector2Ex<float> dimensions, TextStyle typedTextStyle,
            Alignment textAlignment = Alignment::LEFT, AnchorPoint anchorPoint = AnchorPoint::TOP_LEFT,
            float padding = 2.0f, TextStyle backgroundTextStyle = {0.0f, BLACK}, std::string backgroundText = "",
            bool visible = true);

    void Update();
    std::string GetText() const
    {
        return m_text.GetText();
    }

    virtual Vector2Ex<float> GetDimensions() const override;
    virtual void Render(Vector2Ex<float> offset = {0, 0}) const override;

  private:
    void drawCursor(Vector2Ex<float> position, float fontSize, Color color) const;
};
} // namespace UIComponents