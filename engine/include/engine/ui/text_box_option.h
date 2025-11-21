#pragma once
#include "menu_option.h"
#include "text_box.h"
#include <functional>
#include <string>

namespace ui
{
class TextBoxOption : public MenuOption
{
  private:
    TextBox m_textBox;
    TextBoxStyle m_selectedStyle;
    TextBoxStyle m_unselectedStyle;

  protected:
    void UpdateStyle() override;

  public:
    TextBoxOption(Vector2Ex<float> position, Vector2Ex<float> dimensions, const TextBoxStyle& selectedStyle,
                  const TextBoxStyle& unselectedStyle, const std::string& backgroundText = "",
                  AnchorPoint anchorPoint = AnchorPoint::TOP_LEFT, bool visible = true, bool isSelected = false,
                  std::function<void()> callback = nullptr);

    RenderableObject& GetRenderableObject() override;
    const RenderableObject& GetRenderableObject() const override;
};
} // namespace ui