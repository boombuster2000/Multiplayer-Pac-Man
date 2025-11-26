#include "game/layers/create_profile.h"
#include "engine/core/input_manager.h"
#include "engine/ui/enums.h"
#include "engine/ui/text_menu_option.h"
#include "game/components/player.h"
#include "game/components/profile.h"
#include "game/game_application.h"
#include "game/layers/board_selection_menu.h"
#include "game/layers/main_menu.h"
#include "game/layers/profile_selection_menu_layer.h"
#include "game/serialization/json_converters.hpp"
#include <fstream>
#include <nlohmann/json.hpp>


void CreateProfileLayer::SetupMenuOptions()
{
    using namespace ui;
    TextStyle unselectedStyle = {30, GRAY};
    TextStyle selectedStyle = {40, ORANGE};

    m_menu.AddOption(std::make_unique<TextMenuOption>("Continue", selectedStyle, unselectedStyle, false,
                                                      [this]() { this->OnContinueClicked(); }));

    TextStyle backButtonUnselectedStyle = {25, GRAY};
    TextStyle backButtonSelectedStyle = {30, ORANGE};

    m_menu.AddOption(std::make_unique<TextMenuOption>("Back", backButtonSelectedStyle, backButtonUnselectedStyle, false,
                                                      [this]() { this->OnBackClicked(); }));
}

CreateProfileLayer::CreateProfileLayer() : BaseMenuLayer(ui::Alignment::CENTER, true, 10.0f)
{
    using namespace ui;
    const Vector2Ex<float> centreOfScreen = {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};

    TextBoxStyle selectedBoxStyle = {2.0f, ORANGE, WHITE, {20.0f, GRAY}, {20.0f, BLACK}, Alignment::CENTER, 2.0f};
    TextBoxStyle unselectedBoxStyle = {2.0f, BLACK, WHITE, {20.0f, GRAY}, {20.0f, BLACK}, Alignment::CENTER, 2.0f};
    TextBoxStyle activeBoxStyle = {2.0f, DARKBLUE, WHITE, {20.0f, GRAY}, {20.0f, BLACK}, Alignment::CENTER, 2.0f};

    // Create unique_ptr locally, capture raw pointer, then hand ownership to menu
    auto localTextBox = std::make_unique<ui::TextBoxOption>(
        Vector2Ex<float>{200, 50}, selectedBoxStyle, unselectedBoxStyle, activeBoxStyle, "Enter Profile Name",
        ui::AnchorPoint::MIDDLE, false, true, false,
        [this]() { this->m_profileNameInput->SetActive(!(this->m_profileNameInput->IsActive())); });

    m_profileNameInput = localTextBox.get();
    m_menu.AddOption(std::move(localTextBox));
    SetupMenuOptions();
    m_menu.SetPosition({centreOfScreen.x, centreOfScreen.y + 100});
}

void CreateProfileLayer::OnUpdate(float ts)
{
    auto inputManager = engine::Application::GetInputManager();

    if (!m_profileNameInput->IsActive())
        BaseMenuLayer::OnUpdate(ts);
    else
    {

        m_profileNameInput->Update();

        if (inputManager.IsAction("confirm", engine::InputState::PRESSED))
        {
            m_profileNameInput->SetActive(false);
        }
    }
}

void CreateProfileLayer::OnRender()
{
    BaseMenuLayer::OnRender();
}

void CreateProfileLayer::OnTextBoxClicked()
{
}

void CreateProfileLayer::OnContinueClicked()
{
    std::string profileName = m_profileNameInput->GetText();
    if (!profileName.empty())
    {
        auto profile = std::make_shared<Profile>(profileName);
        profile->Save();

        game::GameApplication::Get().SetProfile(profile);
        TransistionTo(std::make_unique<MainMenuLayer>());
    }
}

void CreateProfileLayer::OnBackClicked()
{
    TransistionTo(std::make_unique<ProfileSelectionMenuLayer>());
}