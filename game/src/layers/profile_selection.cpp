#include "engine/core/application.h"
#include "engine/ui/text_menu_option.h"
#include "game/components/profile.h"
#include "game/game_application.h"
#include "game/layers/create_profile.h"
#include "game/layers/main_menu.h"
#include "game/layers/profile_selection_menu_layer.h"
#include "game/serialization/json_converters.hpp"
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

ProfileSelectionMenuLayer::ProfileSelectionMenuLayer() : BaseMenuLayer(ui::Alignment::CENTER, true, 10.0f)
{
    SetupMenuOptions();
}

void ProfileSelectionMenuLayer::SetupMenuOptions()
{
    using namespace ui;
    TextStyle profile_unselectedStyle = {30, BLACK};
    TextStyle profile_selectedStyle = {35, RED};
    TextStyle create_unselectedStyle = {40, BLACK};
    TextStyle create_selectedStyle = {45, RED};

    const std::string path = "profiles";

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".json")
        {
            std::ifstream f(entry.path());
            nlohmann::json data = nlohmann::json::parse(f);
            auto profile = std::make_shared<Profile>(data.get<Profile>());

            m_menu.AddOption(std::make_unique<TextMenuOption>(profile->GetUsername(), profile_selectedStyle,
                                                              profile_unselectedStyle, true, [this, profile]() {
                                                                  game::GameApplication::Get().SetProfile(profile);
                                                                  TransistionTo(std::make_unique<MainMenuLayer>());
                                                              }));
        }
    }

    m_menu.AddOption(
        std::make_unique<TextMenuOption>("Create Profile", create_selectedStyle, create_unselectedStyle, false,
                                         [this]() { TransistionTo(std::make_unique<CreateProfileLayer>()); }));
}