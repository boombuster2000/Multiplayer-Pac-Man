#include "game/layers/game_options_menu.h"
#include "engine/core/application.h"
#include "engine/core/input_manager.h"
#include "engine/ui/text_menu_option.h"
#include "game/layers/game.h"
#include "game/layers/main_menu.h"
#include <memory>

GameOptionsMenuLayer::GameOptionsMenuLayer() : BaseMenuLayer(ui::Alignment::CENTER, true, 10.0f)
{
    SetupMenuOptions();
}

void GameOptionsMenuLayer::SetupMenuOptions()
{
    using namespace ui;
    TextStyle unselectedStyle = {50, BLACK};
    TextStyle selectedStyle = {60, RED};

    m_menu.AddOption(std::make_unique<TextMenuOption>("Resume", selectedStyle, unselectedStyle, true, [this]() {
        GameLayer* gameLayer = engine::Application::Get().GetLayer<GameLayer>();
        if (gameLayer)
        {
            gameLayer->Resume();
            Pop();
        }
    }));

    m_menu.AddOption(std::make_unique<TextMenuOption>("Options", selectedStyle, unselectedStyle, false));

    m_menu.AddOption(std::make_unique<TextMenuOption>("Return To Menu", selectedStyle, unselectedStyle, false, []() {
        GameLayer* gameLayer = engine::Application::Get().GetLayer<GameLayer>();
        if (gameLayer)
        {
            gameLayer->TransistionTo(std::make_unique<MainMenuLayer>());
            engine::Application::QueuePop<GameOptionsMenuLayer>();
        }
    }));
}

void GameOptionsMenuLayer::OnUpdate(float ts)
{
    BaseMenuLayer::OnUpdate(ts);

    auto inputManager = engine::Application::GetInputManager();

    if (inputManager->IsAction("pause", engine::InputState::PRESSED))
    {
        GameLayer* gameLayer = engine::Application::Get().GetLayer<GameLayer>();
        if (gameLayer)
        {
            gameLayer->Resume();
            Pop();
        }
    }
}

void GameOptionsMenuLayer::OnRender()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.45f));

    const float borderWidth = 20;
    Vector2Ex<float> dimensions = m_menu.GetDimensions() + Vector2Ex<float>(2 * borderWidth, 2 * borderWidth);
    Vector2Ex<float> position = m_menu.GetPositionAtAnchor() - Vector2Ex<float>(borderWidth, borderWidth);

    DrawRectangleV(position - Vector2Ex<float>(borderWidth, borderWidth),
                   dimensions + Vector2Ex<float>(2 * borderWidth, 2 * borderWidth), MAROON);
    DrawRectangleV(position, dimensions, WHITE);

    BaseMenuLayer::OnRender();
}
