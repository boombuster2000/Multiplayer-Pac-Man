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
    Text(std::string text, TextStyle style, Vector2Ex<float> anchorPointPosition, AnchorPoint AnchorPoint,
         bool visible);

    void SetText(std::string text);
    std::string GetText() const;

    void SetFontSize(float fontSize);
    float GetFontSize() const;

    void SetColor(Color color);
    Color GetColor() const;

    void SetStyle(TextStyle style);
    TextStyle GetStyle() const;

    Vector2Ex<float> GetDimensions() const override;
    void Render(Vector2Ex<float> offset = {0, 0}) const override;
};

} // namespace ui