#include "Menu.h"
#include <stdexcept>
#include <iostream>


namespace Components { 
    Menu::Menu(Vector2 anchorPointPosition, AnchorPoint anchorPoint, Alignment alignment, bool visible, int spacing)
        : RenderableObject(anchorPointPosition, visible, anchorPoint), m_selectedIndex(0), m_spacing(spacing), m_alignment(alignment)
    {
        UpdateDrawPoint();
        m_options = std::vector<MenuOption>();
    }

    void Menu::AddOption(MenuOption option)
    {
        
        if (!m_options.empty())
        {
            Vector2 newOptionPosition = GetPosition();
            MenuOption& lastOption = m_options.back();
            newOptionPosition.y = lastOption.GetPosition().y + lastOption.GetSize().y + m_spacing; // Move text down
            
            option.SetAnchorPointPosition(newOptionPosition);
            m_options.push_back(option);
        }
        else
        {
            m_options.push_back(option);

            Vector2 newOptionPosition = GetPosition();
            MenuOption& newOption = m_options.back();
            newOption.SetAnchorPointPosition(newOptionPosition);
            newOption.SetSelected(true);
        }
        
    }

    void Menu::DeleteOption(int index)
    {
        if (index < 0 || index >= static_cast<int>(m_options.size()))
            return;

        m_options.erase(m_options.begin() + index);

        if (m_selectedIndex >= static_cast<int>(m_options.size()))
            m_selectedIndex = static_cast<int>(m_options.size()) - 1;

        if (!m_options.empty())
            m_options[m_selectedIndex].SetSelected(true);
    }

    void Menu::ClearOptions()
    {
        m_options.clear();
        m_selectedIndex = 0;
    }

    void Menu::SelectNext()
    {
        if (m_options.empty())
            return;

        m_options[m_selectedIndex].SetSelected(false);
        m_selectedIndex = (m_selectedIndex + 1) % m_options.size();
        m_options[m_selectedIndex].SetSelected(true);
    }

    void Menu::SelectPrevious()
    {
        if (m_options.empty())
            return;

        m_options[m_selectedIndex].SetSelected(false);
        m_selectedIndex = (m_selectedIndex - 1 + m_options.size()) % m_options.size();
        m_options[m_selectedIndex].SetSelected(true);
    }

    const MenuOption& Menu::GetSelectedOption() const
    {
        if (m_options.empty())
            throw std::out_of_range("No options in the menu.");

        return m_options[m_selectedIndex];
    }

    int Menu::GetSelectedIndex() const
    {
        return m_selectedIndex;
    }

    void Menu::UpdateOptionsAnchorPointPositions()
    {
        UpdateDrawPoint();
        Vector2 drawPoint = GetPosition();

        for (int i=0; i<m_options.size(); i++)
        {
            if (i==0)
            {
                m_options[i].SetAnchorPointPosition(drawPoint);
            }
            else
            {
                switch (m_alignment)
                {
                case Alignment::LEFT:
                    m_options[i].SetAnchorPoint(AnchorPoint::TOP_LEFT);
                    break;

                case Alignment::CENTER:
                    m_options[i].SetAnchorPoint(AnchorPoint::TOP_MIDDLE);
                    break;

                case Alignment::RIGHT:
                    m_options[i].SetAnchorPoint(AnchorPoint::TOP_RIGHT);
                    break;

                default:
                    break;
                }


                Vector2 newOptionPosition = (m_alignment == Alignment::LEFT) ? m_options[0].GetPosition() :
                                            (m_alignment == Alignment::CENTER) ? m_options[0].GetPosition(AnchorPoint::TOP_MIDDLE) :
                                            (m_alignment == Alignment::RIGHT) ? m_options[0].GetPosition(AnchorPoint::TOP_RIGHT) :
                                            m_options[0].GetPosition();

                const MenuOption& lastOption = m_options[i-1];

                newOptionPosition.y = lastOption.GetPosition().y + lastOption.GetSize().y + m_spacing; // Move text down
                m_options[i].SetAnchorPointPosition(newOptionPosition);
           }
        }
    }

    Vector2 Menu::GetSize() const
    {
        float width = 0;
        float height = 0;
    

        for (const auto& option : m_options)
        {
            Vector2 optionSize = option.GetSize();
            if (optionSize.x > width)
                width = optionSize.x;
            height += optionSize.y + m_spacing;
        }

        if (!m_options.empty())
            height -= m_spacing; // Remove extra spacing after the last option

        return { width, height };
    }

    void Menu::Render() const
    {
        if (!IsVisible())
            return;

        for (const auto& option : m_options)
        {
            option.Render();
        }
    }
}

