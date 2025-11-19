#include "game/layers/create_profile.h"
#include "engine/ui/enums.h"
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

    m_menu.AddOption(
        MenuOption("Continue", selectedStyle, unselectedStyle, true, true, [this]() { this->OnContinueClicked(); }));
    m_menu.AddOption(
        MenuOption("Back", selectedStyle, unselectedStyle, true, false, [this]() { this->OnBackClicked(); }));
}

CreateProfileLayer::CreateProfileLayer() : BaseMenuLayer(ui::Alignment::CENTER, true, 10.0f)
{
    using namespace ui;
    const Vector2Ex<float> centreOfScreen = {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};

    const TextStyle backgroundTextStyle = {20.0f, GRAY};
    const TextStyle typedTextStyle = {20.0f, BLACK};

    m_profileNameInput =
        std::make_unique<TextBox>(centreOfScreen, Vector2Ex<float>{200, 50}, typedTextStyle, Alignment::CENTER,
                                  AnchorPoint::MIDDLE, 2.0f, backgroundTextStyle, "Enter Profile Name", true);

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
    m_profileNameInput->Render();
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