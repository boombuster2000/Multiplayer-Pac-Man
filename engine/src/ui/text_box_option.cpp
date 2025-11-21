#include "engine/ui/text_box_option.h"

namespace ui
{

TextBoxOption::TextBoxOption(Vector2Ex<float> position, Vector2Ex<float> dimensions, const TextBoxStyle& selectedStyle,
                             const TextBoxStyle& unselectedStyle, const std::string& backgroundText,
                             AnchorPoint anchorPoint, bool visible, bool isSelected, std::function<void()> callback)
    : MenuOption(isSelected, callback),
      m_textBox(position, dimensions, unselectedStyle, backgroundText, anchorPoint, visible),
      m_selectedStyle(selectedStyle), m_unselectedStyle(unselectedStyle)
{
    UpdateStyle();
}

RenderableObject& TextBoxOption::GetRenderableObject()
{
    return m_textBox;
}

const RenderableObject& TextBoxOption::GetRenderableObject() const
{
    return m_textBox;
}

void TextBoxOption::UpdateStyle()
{
    if (IsSelected())
    {
        m_textBox.SetStyle(m_selectedStyle);
    }
    else
    {
        m_textBox.SetStyle(m_unselectedStyle);
    }
}

} // namespace ui
