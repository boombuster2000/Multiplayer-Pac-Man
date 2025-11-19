#include "game/layers/main_menu.h"
#include "engine/ui/enums.h"
#include "engine/ui/text_style.h"
#include "game/game_application.h"
#include "game/layers/board_selection_menu.h"

MainMenuLayer::MainMenuLayer() : BaseMenuLayer(ui::Alignment::CENTER, true, 10.0f)
{
    SetupMenuOptions();
}

void MainMenuLayer::SetupMenuOptions()
{
    using namespace ui;
    TextStyle unselectedStyle = {50, BLACK};
    TextStyle selectedStyle = {60, RED};

    m_menu.AddOption(MenuOption("Start Game", selectedStyle, unselectedStyle, true, true,
                                [this]() { TransistionTo(std::make_unique<BoardSelectionMenuLayer>()); }));

    m_menu.AddOption(MenuOption("Options", selectedStyle, unselectedStyle, true, false, []() {}));

    m_menu.AddOption(
        MenuOption("Exit", selectedStyle, unselectedStyle, true, false, []() { game::GameApplication::Get().Stop(); }));
}