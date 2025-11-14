#include "GameOptionsMenuLayer.h"
#include "GameLayer/GameLayer.h"
#include "MainMenuLayer/MainMenuLayer.h"
#include "Core/Application.h"
#include "Core/InputManager.h"

GameOptionsMenuLayer::GameOptionsMenuLayer()
    : m_menu({(float)GetScreenWidth() / 2,
              (float)GetScreenHeight() / 2},
             UIComponents::AnchorPoint::MIDDLE,
             UIComponents::Alignment::CENTER, true, 10)
{
    using namespace UIComponents;
    TextStyle unselectedStyle = {50, BLACK};
    TextStyle selectedStyle = {60, RED};

    m_menu.AddOption(MenuOption("Resume", selectedStyle, unselectedStyle, true, true, []()
                                {
                                    GameLayer *gameLayer = Core::Application::Get().GetLayer<GameLayer>();
                                    if (gameLayer)
                                    {
                                        gameLayer->Resume();
                                        Core::Application::QueuePop<GameOptionsMenuLayer>();
                                    } }));

    m_menu.AddOption(MenuOption("Options", selectedStyle, unselectedStyle, true, false));

    m_menu.AddOption(MenuOption("Return To Menu", selectedStyle, unselectedStyle, true, false, []()
                                { 
                                    Core::Application::QueueTransition<GameLayer, MainMenuLayer>();
                                Core::Application::QueuePop<GameOptionsMenuLayer>(); }));
}

GameOptionsMenuLayer::~GameOptionsMenuLayer() = default;

void GameOptionsMenuLayer::OnUpdate(float ts)
{
    auto inputManager = Core::Application::GetInputManager();

    if (inputManager->IsAction("move_down", Core::InputState::PRESSED))
    {
        m_menu.SelectNext();
    }
    else if (inputManager->IsAction("move_up", Core::InputState::PRESSED))
    {
        m_menu.SelectPrevious();
    }
    else if (inputManager->IsAction("confirm", Core::InputState::PRESSED))
    {
        m_menu.ConfirmSelection();
    }
    else if (inputManager->IsAction("pause", Core::InputState::PRESSED))
    {
        GameLayer *gameLayer = Core::Application::Get().GetLayer<GameLayer>();
        if (gameLayer)
        {
            gameLayer->Resume();
            Core::Application::QueuePop<GameOptionsMenuLayer>();
        }
    }

    if (m_menu.IsUIUpdateNeeded())
    {
        m_menu.UpdateOptionsAnchorPointPositions();
    }
}

void GameOptionsMenuLayer::OnRender()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.45f));

    const float borderWidth = 20;
    Vector2Ex<float> dimensions = m_menu.GetDimensions() + Vector2Ex<float>(2 * borderWidth, 2 * borderWidth);
    Vector2Ex<float> position = m_menu.GetPositionAtAnchor() - Vector2Ex<float>(borderWidth, borderWidth);

    DrawRectangleV(position - Vector2Ex<float>(borderWidth, borderWidth), dimensions + Vector2Ex<float>(2 * borderWidth, 2 * borderWidth), MAROON);
    DrawRectangleV(position, dimensions, WHITE);

    m_menu.Render();
}
