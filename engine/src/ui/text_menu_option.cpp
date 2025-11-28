#include "engine/ui/text_menu_option.h"

namespace ui
{
TextMenuOption::TextMenuOption(std::string_view text, TextStyle selectedStyle, TextStyle unselectedStyle,
                               bool isSelected, std::function<void()> callback, Vector2Ex<float> position,
                               AnchorPoint anchorPoint, bool isVisible)
    : MenuOption(isSelected, callback),
      m_text(text, isSelected ? selectedStyle : unselectedStyle, position, anchorPoint, isVisible),
      m_selectedStyle(selectedStyle), m_unselectedStyle(unselectedStyle)
{
}

void TextMenuOption::UpdateStyle()
{
    m_text.SetStyle(IsSelected() ? m_selectedStyle : m_unselectedStyle);
}

RenderableObject& TextMenuOption::GetRenderableObject()
{
    // Note there is slicing.
    return m_text;
}

const RenderableObject& TextMenuOption::GetRenderableObject() const
{
    // Note there is slicing.
    return m_text;
}
} // namespace ui
