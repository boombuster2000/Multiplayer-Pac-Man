#include "Menu.h"
#include <iostream>
#include <stdexcept>

namespace UIComponents
{
Menu::Menu(Vector2Ex<float> anchorPointPosition, AnchorPoint anchorPoint, Alignment alignment, bool visible,
           float spacing)
    : RenderableObject(anchorPointPosition, anchorPoint, visible), m_selectedIndex(0), m_spacing(spacing),
      m_alignment(alignment), m_anchorPoint(anchorPoint), m_isUIupdateNeeded(true)
{
}

void Menu::AddOption(MenuOption option)
{
    // If this is the first option, mark it as selected.
    if (m_options.empty())
    {
        option.SetSelected(true);
    }

    m_options.push_back(option);
    m_isUIupdateNeeded = true; // Let the update function handle all positioning.
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

    m_isUIupdateNeeded = true;
}

void Menu::ClearOptions()
{
    m_options.clear();
    m_selectedIndex = 0;

    m_isUIupdateNeeded = true;
}

void Menu::SelectNext()
{
    if (m_options.empty())
        return;

    m_options[m_selectedIndex].SetSelected(false);
    m_selectedIndex = (m_selectedIndex + 1) % m_options.size();
    m_options[m_selectedIndex].SetSelected(true);

    m_isUIupdateNeeded = true;
}

void Menu::SelectPrevious()
{
    if (m_options.empty())
        return;

    m_options[m_selectedIndex].SetSelected(false);
    m_selectedIndex = (m_selectedIndex - 1 + m_options.size()) % m_options.size();
    m_options[m_selectedIndex].SetSelected(true);

    m_isUIupdateNeeded = true;
}

void Menu::ConfirmSelection()
{
    if (m_options.empty())
        return;

    m_options[m_selectedIndex].Select();
}

const MenuOption &Menu::GetSelectedOption() const
{
    if (m_options.empty())
        throw std::out_of_range("No options in the menu.");

    return m_options[m_selectedIndex];
}

int Menu::GetSelectedIndex() const
{
    return m_selectedIndex;
}

bool Menu::IsUIUpdateNeeded() const
{
    return m_isUIupdateNeeded;
}

void Menu::UpdateOptionsAnchorPointPositions()
{
    if (m_options.empty())
        return;

    SetOrigin(m_anchorPoint);

    // 1. Determine the alignment anchor for all options based on the menu's alignment
    AnchorPoint alignmentAnchor;
    switch (m_alignment)
    {
    case Alignment::CENTER:
        alignmentAnchor = AnchorPoint::TOP_MIDDLE;
        break;
    case Alignment::RIGHT:
        alignmentAnchor = AnchorPoint::TOP_RIGHT;
        break;
    case Alignment::LEFT:
        alignmentAnchor = AnchorPoint::TOP_LEFT;
        break;
    default:
        alignmentAnchor = AnchorPoint::TOP_LEFT;
        break;
    }

    // 2. Get the menu's starting world position for that alignment anchor
    Vector2Ex<float> currentPosition = GetPositionAtAnchor(alignmentAnchor);

    // 3. Loop through all options to set their origin and calculate their final position
    for (size_t i = 0; i < m_options.size(); ++i)
    {
        // Set the option's internal object origin to match the menu's alignment
        m_options[i].SetOrigin(alignmentAnchor);

        // For options after the first one, calculate the new vertical position
        if (i > 0)
        {
            const auto &lastOption = m_options[i - 1];
            // The new Y is the previous option's top-left Y + its height + spacing
            currentPosition.y = lastOption.GetPositionAtAnchor().y + lastOption.GetDimensions().y + m_spacing;
        }

        // Set the world position of the option's origin
        m_options[i].SetPosition(currentPosition);
    }

    m_isUIupdateNeeded = false;
}

Vector2Ex<float> Menu::GetDimensions() const
{
    float width = 0;
    float height = 0;

    for (const auto &option : m_options)
    {
        Vector2Ex<float> optionSize = option.GetDimensions();
        if (optionSize.x > width)
            width = optionSize.x;
        height += optionSize.y + m_spacing;
    }

    if (!m_options.empty())
        height -= m_spacing; // Remove extra spacing after the last option

    return {width, height};
}

void Menu::Render(Vector2Ex<float> offset) const
{
    if (!IsVisible())
        return;

    for (const auto &option : m_options)
    {
        option.Render(offset);
    }
}
} // namespace UIComponents
