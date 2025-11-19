#include "engine/core/application.h"
#include "game/layers/create_profile.h"
#include "game/layers/profile_selection_menu_layer.h"
#include <fstream>

ProfileSelectionMenuLayer::ProfileSelectionMenuLayer() : BaseMenuLayer(ui::Alignment::CENTER, true, 10.0f)
{
    SetupMenuOptions();
}

void ProfileSelectionMenuLayer::SetupMenuOptions()
{
    using namespace ui;
    TextStyle unselectedStyle = {50, BLACK};
    TextStyle selectedStyle = {60, RED};

    m_menu.AddOption(MenuOption("Create Profile", selectedStyle, unselectedStyle, true, false,
                                [this]() { TransistionTo(std::make_unique<CreateProfileLayer>()); }));
}