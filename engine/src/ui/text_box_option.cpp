#include "engine/ui/text_box_option.h"

namespace ui
{

TextBoxOption::TextBoxOption(Vector2Ex<float> dimensions, const TextBoxStyle& selectedStyle,
                             const TextBoxStyle& unselectedStyle, const std::string& backgroundText,
                             AnchorPoint anchorPoint, bool isActive, bool visible, bool isSelected,
                             std::function<void()> callback)
    : MenuOption(isSelected, callback),
      m_textBox({0, 0}, dimensions, unselectedStyle, backgroundText, anchorPoint, isActive, visible),
      m_selectedStyle(selectedStyle), m_unselectedStyle(unselectedStyle)
{
    UpdateStyle();
}

void TextBoxOption::Update()
{
    m_textBox.Update(); // Only updates if active, condintion inside this function.
}

bool TextBoxOption::IsActive() const
{
    return m_textBox.IsActive();
}

void TextBoxOption::SetActive(bool isActive)
{
    m_textBox.SetActive(isActive);
}

std::string TextBoxOption::GetText() const
{
    return m_textBox.GetText();
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
