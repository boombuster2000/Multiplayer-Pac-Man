#include "MenuOption.h"

namespace UIComponents {

    MenuOption::MenuOption(std::string text,
            TextStyle selectedStyle,
            TextStyle unselectedStyle,
            bool visible,
            bool isSelected
    ): Text(text, unselectedStyle, {0,0}, AnchorPoint::TOP_LEFT, visible),
       m_selectedStyle(selectedStyle), m_unselectedStyle(unselectedStyle), m_isSelected(isSelected)
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

}