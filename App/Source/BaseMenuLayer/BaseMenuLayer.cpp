#include "BaseMenuLayer.h"
#include "Core/Application.h"
#include "Core/InputManager.h"

BaseMenuLayer::BaseMenuLayer(const UIComponents::Alignment alignment, const bool isVisible, const float spacing)
    : m_menu({(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2}, UIComponents::AnchorPoint::MIDDLE,
             alignment, isVisible, spacing)
{
}

void BaseMenuLayer::OnUpdate(float ts)
{
    auto inputManager = Core::Application::GetInputManager();

    if (inputManager->IsAction("move_down", Core::InputState::PRESSED))
    {
        m_menu.SelectNext();
    }
    else if (inputManager->IsAction("move_up", Core::InputState::PRESSED))
    {
        m_menu.SelectPrevious();
    }
    else if (inputManager->IsAction("confirm", Core::InputState::PRESSED))
    {
        m_menu.ConfirmSelection();
    }

    if (m_menu.IsUIUpdateNeeded())
    {
        m_menu.UpdateOptionsAnchorPointPositions();
    }
}

void BaseMenuLayer::OnRender()
{
    m_menu.Render();
}
