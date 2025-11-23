#include "game/layers/board_selection_menu.h"
#include "engine/core/application.h"
#include "engine/ui/text_menu_option.h"
#include "game/layers/game.h"
#include "game/layers/main_menu.h"
#include <filesystem>
#include <iostream>
#include <string>

BoardSelectionMenuLayer::BoardSelectionMenuLayer() : BaseMenuLayer(ui::Alignment::CENTER, true, 10.0f)
{
    SetupMenuOptions();
}

void BoardSelectionMenuLayer::SetupMenuOptions()
{
    using namespace ui;
    TextStyle boardUnselectedStyle = {30, DARKGRAY};
    TextStyle boardSelectedStyle = {40, ORANGE};

    m_menu.AddOption(std::make_unique<TextMenuOption>("built-in", boardSelectedStyle, boardUnselectedStyle, true,
                                                      [this]() { TransistionTo(std::make_unique<GameLayer>()); }));

    const std::string path = "./resources/boards/";
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".json")
        {
            std::string filename = entry.path().stem().string();
            std::string fullPath = entry.path().string();
            m_menu.AddOption(std::make_unique<TextMenuOption>(
                filename, boardSelectedStyle, boardUnselectedStyle, false,
                [this, fullPath]() { TransistionTo(std::make_unique<GameLayer>(fullPath)); }));
        }
    }

    TextStyle backButtonUnselectedStyle = {25, GRAY};
    TextStyle backButtonSelectedStyle = {30, LIGHTGRAY};

    m_menu.AddOption(std::make_unique<TextMenuOption>("Back", backButtonSelectedStyle, backButtonUnselectedStyle, false,
                                                      [this]() { TransistionTo(std::make_unique<MainMenuLayer>()); }));
}
