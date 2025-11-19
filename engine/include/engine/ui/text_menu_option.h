#pragma once
#include "engine/core/vector2ex.h"
#include "menu_option.h"
#include "text.h"
#include "text_style.h"
#include <string>

namespace ui
{
class TextMenuOption : public MenuOption
{
  private:
    Text m_text;
    TextStyle m_selectedStyle;
    TextStyle m_unselectedStyle;

  public:
    TextMenuOption(std::string text, TextStyle selectedStyle, TextStyle unselectedStyle, bool isSelected = false,
                   std::function<void()> callback = nullptr, Vector2Ex<float> position = {0, 0},
                   AnchorPoint anchorPoint = AnchorPoint::TOP_LEFT, bool isVisible = true);

    virtual void UpdateStyle() override;

    virtual RenderableObject& GetRenderableObject() override;
    virtual const RenderableObject& GetRenderableObject() const override;
};
} // namespace ui