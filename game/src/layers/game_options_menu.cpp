#include "game/layers/game_options_menu.h"
#include "engine/core/input_manager.h"
#include "engine/ui/text_menu_option.h"
#include "game/game_application.h"
#include "game/layers/game.h"
#include "game/layers/main_menu.h"
#include <memory>

GameOptionsMenuLayer::GameOptionsMenuLayer() :
    BaseMenuLayer(ui::Alignment::CENTER, true, 10.0f)
{
    SetupMenuOptions();
}

void GameOptionsMenuLayer::SetupMenuOptions()
{
    using namespace ui;
    TextStyle unselectedStyle = {30, DARKGRAY};
    TextStyle selectedStyle = {40, ORANGE};

    m_menu.AddOption(std::make_unique<TextMenuOption>("Resume", selectedStyle, unselectedStyle, true, [this]() {
        if (GameLayer* gameLayer = game::GameApplication::Get().GetLayer<GameLayer>())
        {
            gameLayer->Resume();
            Pop();
        }
    }));

    m_menu.AddOption(std::make_unique<TextMenuOption>("Return To Menu", selectedStyle, unselectedStyle, false, []() {
        if (const GameLayer* gameLayer = game::GameApplication::Get().GetLayer<GameLayer>())
        {
            gameLayer->TransistionTo(std::make_unique<MainMenuLayer>());
            engine::Application::QueuePop<GameOptionsMenuLayer>();
        }
    }));
}

void GameOptionsMenuLayer::OnUpdate(float ts)
{
    BaseMenuLayer::OnUpdate(ts);

    if (const auto& inputManager = engine::Application::GetInputManager();
        inputManager.IsAction("pause", engine::InputState::PRESSED))
    {
        if (GameLayer* gameLayer = game::GameApplication::Get().GetLayer<GameLayer>())
        {
            gameLayer->Resume();
            Pop();
        }
    }
}

void GameOptionsMenuLayer::OnRender()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.45f));

    constexpr float borderWidth = 20;
    const Vector2Ex<float> dimensions = m_menu.GetDimensions() + Vector2Ex<float>(2 * borderWidth, 2 * borderWidth);
    const Vector2Ex<float> position = m_menu.GetPositionAtAnchor() - Vector2Ex<float>(borderWidth, borderWidth);

    DrawRectangleV(position - Vector2Ex<float>(borderWidth, borderWidth),
                   dimensions + Vector2Ex<float>(2 * borderWidth, 2 * borderWidth),
                   MAROON);
    DrawRectangleV(position, dimensions, WHITE);

    BaseMenuLayer::OnRender();
}
