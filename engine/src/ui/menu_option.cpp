#include "engine/ui/menu_option.h"

namespace ui
{
MenuOption::MenuOption(bool isSelected, std::function<void()> callback)
    : m_isSelected(isSelected), m_callback(std::move(callback))
{
}

void MenuOption::SetSelected(const bool selected)
{
    m_isSelected = selected;
    UpdateStyle();
}

bool MenuOption::IsSelected() const
{
    return m_isSelected;
}

void MenuOption::Select() const
{
    if (m_callback)
        m_callback();
}
} // namespace ui