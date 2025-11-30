#pragma once

#include <string>

#include "engine/core/vector2ex.h"
#include "engine/ui/enums.h"
#include "engine/ui/renderable_object.h"
#include "engine/ui/text_style.h"
#include "raylib.h"

namespace ui
{

class Text : public RenderableObject
{
  private:
    std::string m_text;
    TextStyle m_style;

  public:
    Text(std::string_view text, TextStyle style, Vector2Ex<float> anchorPointPosition, AnchorPoint anchorPoint,
         bool visible);

    void SetText(std::string_view text);
    std::string GetText() const;

    void SetFontSize(const float fontSize);
    float GetFontSize() const;

    void SetColor(const Color& color);
    Color GetColor() const;

    void SetStyle(const TextStyle& style);
    TextStyle GetStyle() const;

    Vector2Ex<float> GetDimensions() const override;
    void Render(Vector2Ex<float> offset = {0, 0}) const override;
};

} // namespace ui