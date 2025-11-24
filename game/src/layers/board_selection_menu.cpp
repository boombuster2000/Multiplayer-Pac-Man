#include "game/layers/board_selection_menu.h"
#include "engine/core/application.h"
#include "engine/core/input_manager.h"
#include "engine/ui/text_menu_option.h"
#include "game/layers/game.h"
#include "game/layers/main_menu.h"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>

BoardSelectionMenuLayer::BoardSelectionMenuLayer()
    : m_menu({(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2}, ui::AnchorPoint::TOP_LEFT,
             ui::Alignment::CENTER, true, 10.0f),
      m_leaderboardTitle("Leaderboard", ui::TextStyle{40, ORANGE},
                         {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2}, ui::AnchorPoint::TOP_LEFT, true)
{
    SetupMenuOptions();
    UpdateLeaderboard();
}

void BoardSelectionMenuLayer::SetupMenuOptions()
{
    using namespace ui;
    TextStyle boardUnselectedStyle = {30, DARKGRAY};
    TextStyle boardSelectedStyle = {40, ORANGE};

    m_boardPaths.push_back("built-in");
    m_menu.AddOption(std::make_unique<TextMenuOption>("built-in", boardSelectedStyle, boardUnselectedStyle, true,
                                                      [this]() { TransistionTo(std::make_unique<GameLayer>()); }));

    const std::string path = "./resources/boards/";
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".json")
        {
            std::string filename = entry.path().stem().string();
            std::string fullPath = entry.path().string();
            m_boardPaths.push_back(fullPath);
            m_menu.AddOption(std::make_unique<TextMenuOption>(
                filename, boardSelectedStyle, boardUnselectedStyle, false,
                [this, fullPath]() { TransistionTo(std::make_unique<GameLayer>(fullPath)); }));
        }
    }

    TextStyle backButtonUnselectedStyle = {25, GRAY};
    TextStyle backButtonSelectedStyle = {30, ORANGE};

    m_boardPaths.push_back("back"); // placeholder for back button
    m_menu.AddOption(std::make_unique<TextMenuOption>("Back", backButtonSelectedStyle, backButtonUnselectedStyle, false,
                                                      [this]() { TransistionTo(std::make_unique<MainMenuLayer>()); }));
}

void BoardSelectionMenuLayer::OnUpdate(float ts)
{
    auto inputManager = engine::Application::GetInputManager();

    if (inputManager->IsAction("move_down", engine::InputState::PRESSED))
    {
        m_menu.SelectNext();
    }
    else if (inputManager->IsAction("move_up", engine::InputState::PRESSED))
    {
        m_menu.SelectPrevious();
    }
    else if (inputManager->IsAction("confirm", engine::InputState::PRESSED))
    {
        m_menu.ConfirmSelection();
    }

    if (m_lastSelectedIndex != m_menu.GetSelectedIndex())
    {
        UpdateLeaderboard();
    }

    if (m_menu.IsUIUpdateNeeded())
    {
        PositionUIElements();
        m_menu.UpdateOptionsAnchorPointPositions();
    }
}

void BoardSelectionMenuLayer::UpdateLeaderboard()
{
    m_lastSelectedIndex = m_menu.GetSelectedIndex();

    Board board;
    if (m_boardPaths[m_lastSelectedIndex] != "built-in" && m_boardPaths[m_lastSelectedIndex] != "back")
    {
        board = Board::LoadFromFile(m_boardPaths[m_lastSelectedIndex]);
    }

    auto highscores = board.GetHighscores();
    std::vector<std::pair<std::string, int>> sortedScores(highscores.begin(), highscores.end());
    std::sort(sortedScores.begin(), sortedScores.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    m_leaderboardScores.clear();
    m_leaderboardWidth = m_leaderboardTitle.GetDimensions().x;

    for (const auto& score : sortedScores)
    {
        m_leaderboardScores.emplace_back(score.first + " " + std::to_string(score.second), ui::TextStyle{30, LIGHTGRAY},
                                         Vector2Ex<float>{0, 0}, ui::AnchorPoint::TOP_LEFT, true);
        if (m_leaderboardScores.back().GetDimensions().x > m_leaderboardWidth)
        {
            m_leaderboardWidth = m_leaderboardScores.back().GetDimensions().x;
        }
    }
    PositionUIElements();
}

void BoardSelectionMenuLayer::PositionUIElements()
{
    float menuWidth = m_menu.GetDimensions().x;
    float menuHeight = m_menu.GetDimensions().y;
    float spacing = 50.f;

    // Leaderboard dimensions
    float leaderboardHeight = m_leaderboardTitle.GetDimensions().y;
    if (!m_leaderboardScores.empty())
    {
        leaderboardHeight += 10.f; // spacing
        for (const auto& score : m_leaderboardScores)
        {
            leaderboardHeight += 5.f + score.GetDimensions().y;
        }
    }

    float totalWidth = menuWidth + spacing + m_leaderboardWidth;
    float startX = ((float)GetScreenWidth() - totalWidth) / 2;

    // Position Menu
    m_menu.SetPosition({startX, ((float)GetScreenHeight() - menuHeight) / 2});
    m_menu.SetOrigin(ui::AnchorPoint::TOP_LEFT);

    // Position Leaderboard
    float leaderboardStartY = ((float)GetScreenHeight() - leaderboardHeight) / 2;
    Vector2Ex leaderboardTitlePos = {startX + menuWidth + spacing, leaderboardStartY};
    m_leaderboardTitle.SetPosition(leaderboardTitlePos);
    m_leaderboardTitle.SetOrigin(ui::AnchorPoint::TOP_LEFT);

    Vector2Ex currentScorePos = m_leaderboardTitle.GetPositionAtAnchor(ui::AnchorPoint::BOTTOM_LEFT);
    currentScorePos.y += 10.f; // spacing between title and scores

    for (auto& score : m_leaderboardScores)
    {
        score.SetPosition(currentScorePos);
        score.SetOrigin(ui::AnchorPoint::TOP_LEFT);
        currentScorePos.y += score.GetDimensions().y + 5.f;
    }
}

void BoardSelectionMenuLayer::OnRender()
{
    m_menu.Render();

    if (!m_leaderboardScores.empty())
    {
        m_leaderboardTitle.Render();
        for (const auto& score : m_leaderboardScores)
        {
            score.Render();
        }
    }
}
