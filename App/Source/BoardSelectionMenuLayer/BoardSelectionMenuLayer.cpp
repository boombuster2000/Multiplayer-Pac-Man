#include "BoardSelectionMenuLayer.h"
#include "Core/Application.h"
#include "GameLayer/GameLayer.h"
#include "MainMenuLayer/MainMenuLayer.h"
#include <filesystem>
#include <iostream>
#include <string>

BoardSelectionMenuLayer::BoardSelectionMenuLayer()
    : m_menu({(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2}, UIComponents::AnchorPoint::MIDDLE,
             UIComponents::Alignment::CENTER, true, 10)
{
    using namespace UIComponents;
    TextStyle unselectedStyle = {50, BLACK};
    TextStyle selectedStyle = {60, RED};

    m_menu.AddOption(MenuOption("built-in", selectedStyle, unselectedStyle, true, true,
                                [this]() { TransitionTo(std::make_unique<GameLayer>()); }));

    const std::string path = "./Boards/";
    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".json")
        {
            std::string filename = entry.path().stem().string();
            std::string fullPath = entry.path().string();
            m_menu.AddOption(MenuOption(filename, selectedStyle, unselectedStyle, true, false,
                                        [this, fullPath]() { TransitionTo(std::make_unique<GameLayer>(fullPath)); }));
        }
    }

    m_menu.AddOption(MenuOption("Back", selectedStyle, unselectedStyle, true, false,
                                [this]() { TransitionTo(std::make_unique<MainMenuLayer>()); }));
}

BoardSelectionMenuLayer::~BoardSelectionMenuLayer() = default;

void BoardSelectionMenuLayer::OnUpdate(float ts)
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

void BoardSelectionMenuLayer::OnRender()
{
    m_menu.Render();
}
