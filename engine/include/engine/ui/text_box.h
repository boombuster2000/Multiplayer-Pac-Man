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
    float m_cursorTimer = 0.0f;
    Vector2Ex<float> m_dimensions;
    Text m_text;
    Text m_backgroundText;
    bool m_cursorVisible;
    TextBoxStyle m_style;
    TextBoxStyle m_activeStyle;
    TextBoxStyle m_renderStyle;
    bool m_isActive;

  private:
    void HandleKeyPresses();
    void UpdateCursor();
    void UpdateAllTextPositions();
    void DrawCursor(Vector2Ex<float> position, float fontSize, Color color) const;
    Vector2Ex<float> CalculateTextPositionOffset(const Vector2Ex<float>& boxDimensions,
                                                 const Vector2Ex<float>& textDimensions) const;

  public:
    TextBox(Vector2Ex<float> position, Vector2Ex<float> dimensions, const TextBoxStyle& style,
            const TextBoxStyle& activeStyle, std::string_view backgroundText = "",
            AnchorPoint anchorPoint = AnchorPoint::TOP_LEFT, bool isActive = false, bool visible = true);

    void SetStyle(const TextBoxStyle& style);
    std::string GetText() const;
    void Update();

    bool IsActive() const;
    void SetActive(bool isActive);

    void SetPosition(Vector2Ex<float> position) override;
    Vector2Ex<float> GetDimensions() const override;
    void Render(Vector2Ex<float> offset) const override;
};
} // namespace ui