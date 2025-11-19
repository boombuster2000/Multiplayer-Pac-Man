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
    TextStyle unselectedStyle = {50, BLACK};
    TextStyle selectedStyle = {60, RED};

    m_menu.AddOption(std::make_unique<TextMenuOption>("built-in", selectedStyle, unselectedStyle, true,
                                                      [this]() { TransistionTo(std::make_unique<GameLayer>()); }));

    const std::string path = "./resources/boards/";
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".json")
        {
            std::string filename = entry.path().stem().string();
            std::string fullPath = entry.path().string();
            m_menu.AddOption(
                std::make_unique<TextMenuOption>(filename, selectedStyle, unselectedStyle, false, [this, fullPath]() {
                    TransistionTo(std::make_unique<GameLayer>(fullPath));
                }));
        }
    }

    m_menu.AddOption(std::make_unique<TextMenuOption>("Back", selectedStyle, unselectedStyle, false,
                                                      [this]() { TransistionTo(std::make_unique<MainMenuLayer>()); }));
}
