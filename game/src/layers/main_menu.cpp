#include "game/layers/main_menu.h"
#include "engine/ui/enums.h"
#include "engine/ui/text_menu_option.h"
#include "engine/ui/text_style.h"
#include "game/game_application.h"
#include "game/layers/board_selection_menu.h"
#include "game/layers/profile_selection_menu_layer.h"

MainMenuLayer::MainMenuLayer() :
    BaseMenuLayer(ui::Alignment::CENTER, true, 10.0f)
{
    SetupMenuOptions();
}

void MainMenuLayer::SetupMenuOptions()
{
    using namespace ui;
    TextStyle unselectedStyle = {30, DARKGRAY};
    TextStyle selectedStyle = {40, ORANGE};

    m_menu.AddOption(std::make_unique<TextMenuOption>("Start Game", selectedStyle, unselectedStyle, true, [this]() {
        TransistionTo(std::make_unique<BoardSelectionMenuLayer>());
    }));

    m_menu.AddOption(
        std::make_unique<TextMenuOption>("Profile Selection", selectedStyle, unselectedStyle, false, [this]() {
            TransistionTo(std::make_unique<ProfileSelectionMenuLayer>());
        }));

    m_menu.AddOption(std::make_unique<TextMenuOption>("Exit", selectedStyle, unselectedStyle, false, []() {
        game::GameApplication::Get().Stop();
    }));
}