#include "BoardSelectionMenuLayer.h"
#include "Core/Application.h"
#include "GameLayer/GameLayer.h"
#include "MainMenuLayer/MainMenuLayer.h"
#include <filesystem>
#include <iostream>
#include <string>

BoardSelectionMenuLayer::BoardSelectionMenuLayer() : BaseMenuLayer(UIComponents::Alignment::CENTER, true, 10.0f)
{
    SetupMenuOptions();
}

void BoardSelectionMenuLayer::SetupMenuOptions()
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
