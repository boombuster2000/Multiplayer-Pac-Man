#include "GameOptionsLayer.h"
#include "GameComponents/GameLayer.h"
#include "MainMenuComponents/MainMenuLayer.h"
#include "Core/Application.h"
#include "Core/InputManager.h"

GameOptionsLayer::GameOptionsLayer()
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
                                        Core::Application::QueuePop<GameOptionsLayer>();
                                    } }));

    m_menu.AddOption(MenuOption("Options", selectedStyle, unselectedStyle, true, false));

    m_menu.AddOption(MenuOption("Return To Menu", selectedStyle, unselectedStyle, true, false, []()
                                { 
                                    Core::Application::QueueTransition<GameLayer, MainMenuLayer>();
                                Core::Application::QueuePop<GameOptionsLayer>(); }));
}

GameOptionsLayer::~GameOptionsLayer() = default;

void GameOptionsLayer::OnUpdate(float ts)
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

    if (m_menu.IsUIUpdateNeeded())
    {
        m_menu.UpdateOptionsAnchorPointPositions();
    }
}

void GameOptionsLayer::OnRender()
{
    ClearBackground(WHITE);
    m_menu.Render();
}
