#include "GameOptionsMenuLayer.h"
#include "Core/Application.h"
#include "Core/InputManager.h"
#include "GameLayer/GameLayer.h"
#include "MainMenuLayer/MainMenuLayer.h"

GameOptionsMenuLayer::GameOptionsMenuLayer() : BaseMenuLayer(UIComponents::Alignment::CENTER, true, 10.0f)
{
    SetupMenuOptions();
}

void GameOptionsMenuLayer::SetupMenuOptions()
{
    using namespace UIComponents;
    TextStyle unselectedStyle = {50, BLACK};
    TextStyle selectedStyle = {60, RED};

    m_menu.AddOption(MenuOption("Resume", selectedStyle, unselectedStyle, true, true, [this]() {
        GameLayer *gameLayer = Core::Application::Get().GetLayer<GameLayer>();
        if (gameLayer)
        {
            gameLayer->Resume();
            Pop();
        }
    }));

    m_menu.AddOption(MenuOption("Options", selectedStyle, unselectedStyle, true, false));

    m_menu.AddOption(MenuOption("Return To Menu", selectedStyle, unselectedStyle, true, false, []() {
        GameLayer *gameLayer = Core::Application::Get().GetLayer<GameLayer>();
        if (gameLayer)
        {
            gameLayer->TransitionTo(std::make_unique<MainMenuLayer>());
            Core::Application::QueuePop<GameOptionsMenuLayer>();
        }
    }));
}

void GameOptionsMenuLayer::OnUpdate(float ts)
{
    BaseMenuLayer::OnUpdate(ts);

    auto inputManager = Core::Application::GetInputManager();

    if (inputManager->IsAction("pause", Core::InputState::PRESSED))
    {
        GameLayer *gameLayer = Core::Application::Get().GetLayer<GameLayer>();
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
