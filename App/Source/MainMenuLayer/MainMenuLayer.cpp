#include "MainMenuLayer.h"
#include "BoardSelectionMenuLayer/BoardSelectionMenuLayer.h"
#include "Core/Application.h"
#include "UIComponents/Enums.h"
#include "UIComponents/TextStyle.h"
#include <iostream>

MainMenuLayer::MainMenuLayer() : BaseMenuLayer(UIComponents::Alignment::CENTER, true, 10.0f)
{
    SetupMenuOptions();
}

void MainMenuLayer::SetupMenuOptions()
{
    using namespace UIComponents;
    TextStyle unselectedStyle = {50, BLACK};
    TextStyle selectedStyle = {60, RED};

    m_menu.AddOption(MenuOption("Start Game", selectedStyle, unselectedStyle, true, true,
                                [this]() { TransitionTo(std::make_unique<BoardSelectionMenuLayer>()); }));

    m_menu.AddOption(MenuOption("Options", selectedStyle, unselectedStyle, true, false,
                                []() { std::cout << "Options" << std::endl; }));

    m_menu.AddOption(
        MenuOption("Exit", selectedStyle, unselectedStyle, true, false, []() { Core::Application::Get().Stop(); }));
}