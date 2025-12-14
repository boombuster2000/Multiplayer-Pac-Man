#include "game/layers/game_over_layer.h"
#include "engine/core/input_manager.h"
#include "engine/ui/text_menu_option.h"
#include "game/game_application.h"
#include "game/layers/game.h"
#include "game/layers/main_menu.h"
#include <format>
#include <numeric>

GameOverLayer::GameOverLayer(const std::vector<Client>& clients) :
    m_menu({0, 0}, ui::AnchorPoint::MIDDLE, ui::Alignment::CENTER, true, 20.0f),
    m_title("Game Over",
            {60, RED},
            {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 4},
            ui::AnchorPoint::MIDDLE,
            true)
{
    SetupUI(clients);
}

void GameOverLayer::SetupUI(const std::vector<Client>& clients)
{
    // --- Scores ---
    float totalWidth = 0;
    constexpr float padding = 50.0f; // Padding between each player's score column
    float maxColumnHeight = 0;

    // 1. Create Text objects and calculate column widths
    for (const auto& client : clients)
    {
        ui::Text usernameText(std::string(client.profile->GetUsername()),
                              ui::TextStyle{35, client.pacman.GetColor()},
                              {0, 0},
                              ui::AnchorPoint::TOP_MIDDLE,
                              true);

        ui::Text scoreText(std::to_string(client.player.GetPoints()),
                           ui::TextStyle{30, client.pacman.GetColor()},
                           {0, 0},
                           ui::AnchorPoint::TOP_MIDDLE,
                           true);

        float colWidth = std::max(usernameText.GetDimensions().x, scoreText.GetDimensions().x);
        m_playerScoreUIs.push_back({std::move(usernameText), std::move(scoreText), colWidth});

        float colHeight = m_playerScoreUIs.back().username.GetDimensions().y +
                          m_playerScoreUIs.back().score.GetDimensions().y + 10.0f;
        if (colHeight > maxColumnHeight)
        {
            maxColumnHeight = colHeight;
        }
    }

    // 2. Calculate total width of all columns
    if (!m_playerScoreUIs.empty())
    {
        totalWidth =
            std::accumulate(m_playerScoreUIs.begin(), m_playerScoreUIs.end(), 0.0f, [](float sum, const auto& ui) {
                return sum + ui.columnWidth;
            });
        totalWidth += padding * (m_playerScoreUIs.size() - 1);
    }

    // 3. Position the scores
    float currentX = (GetScreenWidth() - totalWidth) / 2.0f;
    const float yPos = (GetScreenHeight() / 2.0f) - 50.0f;

    for (auto& ui : m_playerScoreUIs)
    {
        // Center the username and score text within their column
        float columnStartX = currentX;
        ui.username.SetPosition({columnStartX + ui.columnWidth / 2.0f, yPos});
        ui.score.SetPosition({columnStartX + ui.columnWidth / 2.0f, yPos + ui.username.GetDimensions().y + 10.0f});

        currentX += ui.columnWidth + padding;
    }

    // --- Menu Button ---
    using namespace ui;
    TextStyle unselectedStyle = {30, DARKGRAY};
    TextStyle selectedStyle = {40, ORANGE};

    m_menu.AddOption(std::make_unique<TextMenuOption>("Back to Menu", selectedStyle, unselectedStyle, true, []() {
        GameLayer* gameLayer = game::GameApplication::Get().GetLayer<GameLayer>();
        if (gameLayer)
        {
            // Since GameLayer is suspended, we trigger the transition from it.
            // This will replace GameLayer with MainMenuLayer in the stack.
            gameLayer->TransistionTo(std::make_unique<MainMenuLayer>());
        }
        // Then we pop ourself off the top.
        engine::Application::QueuePop<GameOverLayer>();
    }));

    m_menu.SetPosition({(float)GetScreenWidth() / 2, yPos + maxColumnHeight + 80.0f});
    m_menu.UpdateOptionsAnchorPointPositions();
}

void GameOverLayer::OnUpdate(float ts)
{
    const auto& inputManager = engine::Application::GetInputManager();

    if (inputManager.IsAction("move_down", engine::InputState::PRESSED))
    {
        m_menu.SelectNext();
    }
    else if (inputManager.IsAction("move_up", engine::InputState::PRESSED))
    {
        m_menu.SelectPrevious();
    }
    else if (inputManager.IsAction("confirm", engine::InputState::PRESSED))
    {
        m_menu.ConfirmSelection();
    }

    if (m_menu.IsUIUpdateNeeded())
    {
        m_menu.UpdateOptionsAnchorPointPositions();
    }
}

void GameOverLayer::OnRender()
{
    // This layer should be on top of the suspended GameLayer.
    // We can either let GameLayer render one last time or draw our own background.
    // Drawing a semi-transparent background is a common "pause" or "game over" effect.
    // DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);

    m_title.Render();

    for (const auto& ui : m_playerScoreUIs)
    {
        ui.username.Render();
        ui.score.Render();
    }

    m_menu.Render();
}
