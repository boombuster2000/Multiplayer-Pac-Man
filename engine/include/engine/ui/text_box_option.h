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
    TextBoxOption(Vector2Ex<float> dimensions, const TextBoxStyle& selectedStyle, const TextBoxStyle& unselectedStyle,
                  const TextBoxStyle& activeStyle, const std::string& backgroundText = "",
                  AnchorPoint anchorPoint = AnchorPoint::TOP_LEFT, bool isActive = false, bool visible = true,
                  bool isSelected = false, std::function<void()> callback = nullptr);

    void Update();

    bool IsActive() const;
    void SetActive(bool isActive);

    std::string GetText() const;
    RenderableObject& GetRenderableObject() override;
    const RenderableObject& GetRenderableObject() const override;
};
} // namespace ui