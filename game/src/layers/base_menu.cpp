#include "game/layers/base_menu.h"
#include "engine/core/application.h"
#include "engine/core/input_manager.h"

BaseMenuLayer::BaseMenuLayer(const ui::Alignment alignment, const bool IsVisible, const float spacing)
    : m_menu({(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2}, ui::AnchorPoint::MIDDLE, alignment, IsVisible,
             spacing)
{
}

void BaseMenuLayer::OnUpdate(float ts)
{
    auto inputManager = engine::Application::GetInputManager();

    if (inputManager->IsAction("move_down", engine::InputState::PRESSED))
    {
        m_menu.SelectNext();
    }
    else if (inputManager->IsAction("move_up", engine::InputState::PRESSED))
    {
        m_menu.SelectPrevious();
    }
    else if (inputManager->IsAction("confirm", engine::InputState::PRESSED))
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
