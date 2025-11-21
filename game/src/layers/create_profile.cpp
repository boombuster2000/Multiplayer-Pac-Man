#include "game/layers/create_profile.h"
#include "engine/ui/enums.h"
#include "engine/ui/text_menu_option.h"
#include "game/components/player.h"
#include "game/game_application.h"
#include "game/layers/board_selection_menu.h"
#include "game/layers/main_menu.h"
#include "game/layers/profile_selection_menu_layer.h"

void CreateProfileLayer::SetupMenuOptions()
{
    using namespace ui;
    TextStyle unselectedStyle = {20, BLACK};
    TextStyle selectedStyle = {25, RED};

    m_menu.AddOption(std::make_unique<TextMenuOption>("Continue", selectedStyle, unselectedStyle, true,
                                                      [this]() { this->OnContinueClicked(); }));
    m_menu.AddOption(std::make_unique<TextMenuOption>("Back", selectedStyle, unselectedStyle, false,
                                                      [this]() { this->OnBackClicked(); }));
}

CreateProfileLayer::CreateProfileLayer() : BaseMenuLayer(ui::Alignment::CENTER, true, 10.0f)
{
    using namespace ui;
    const Vector2Ex<float> centreOfScreen = {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};

    TextBoxStyle style = {2.0f, BLACK, WHITE, {20.0f, GRAY}, {20.0f, BLACK}, Alignment::CENTER, 2.0f};

    m_profileNameInput = std::make_unique<TextBox>(centreOfScreen, Vector2Ex<float>{200, 50}, style,
                                                   "Enter Profile Name", AnchorPoint::MIDDLE, true);

    SetupMenuOptions();
    m_menu.SetPosition({centreOfScreen.x, centreOfScreen.y + 100});
}

void CreateProfileLayer::OnUpdate(float ts)
{
    BaseMenuLayer::OnUpdate(ts);
    m_profileNameInput->Update();
}

void CreateProfileLayer::OnRender()
{
    BaseMenuLayer::OnRender();
    m_profileNameInput->Render({0, 0});
}

void CreateProfileLayer::OnContinueClicked()
{
    std::string profileName = m_profileNameInput->GetText();
    if (!profileName.empty())
    {
        auto profile = std::make_shared<Profile>(profileName);
        game::GameApplication::Get().SetProfile(profile);
        TransistionTo(std::make_unique<MainMenuLayer>());
    }
}

void CreateProfileLayer::OnBackClicked()
{
    TransistionTo(std::make_unique<ProfileSelectionMenuLayer>());
}