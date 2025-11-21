#pragma once
#include "engine/core/vector2ex.h"
#include "renderable_object.h"
#include "text.h"
#include "text_box_style.h"
#include <string>

namespace ui
{
class TextBox : public RenderableObject
{

  private:
    Vector2Ex<float> m_dimensions;
    Text m_text;
    Text m_backgroundText;
    bool m_cursorVisible;
    float m_cursorTimer;
    TextBoxStyle m_style;

  private:
    void HandleKeyPresses();
    void UpdateCursor();
    void UpdateTextPosition();
    void DrawCursor(Vector2Ex<float> position, float fontSize, Color color) const;
    Vector2Ex<float> CalculateTextPositionOffset(const Vector2Ex<float>& boxDimensions,
                                                 const Vector2Ex<float>& textDimensions) const;

  public:
    TextBox(Vector2Ex<float> position, Vector2Ex<float> dimensions, const TextBoxStyle& style,
            const std::string& backgroundText = "", AnchorPoint anchorPoint = AnchorPoint::TOP_LEFT,
            bool visible = true);

    void Update();
    void Render(Vector2Ex<float> offset) const override;
    Vector2Ex<float> GetDimensions() const override;
    void SetStyle(const TextBoxStyle& style);
    std::string GetText() const;
};
} // namespace ui