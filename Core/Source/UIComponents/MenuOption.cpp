#include "MenuOption.h"

namespace UIComponents
{

    MenuOption::MenuOption(std::string text,
        TextStyle selectedStyle,
        TextStyle unselectedStyle,
        bool visible,
        bool isSelected,
        std::function<void()> callback) : Text(text, unselectedStyle, {0, 0}, AnchorPoint::TOP_LEFT, visible),
        m_selectedStyle(selectedStyle), m_unselectedStyle(unselectedStyle), m_isSelected(isSelected), m_callback(callback)
    {
    }

    void MenuOption::SetSelected(bool isSelected)
    {
        m_isSelected = isSelected;

        if (m_isSelected)
        {
            SetStyle(m_selectedStyle);
        }
        else
        {
            SetStyle(m_unselectedStyle);
        }
    }

    bool MenuOption::IsSelected() const
    {
        return m_isSelected;
    }

    void MenuOption::Select()
    {
        if (m_callback)
        {
            m_callback();
        }
    }
}