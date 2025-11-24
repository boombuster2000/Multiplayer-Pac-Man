#pragma once

#include "engine/core/layer.h"
#include "engine/ui/menu.h"
#include "engine/ui/text.h"
#include "game/components/board.h"
#include <string>
#include <vector>

class BoardSelectionMenuLayer : public engine::Layer
{
  private:
    ui::Menu m_menu;
    std::vector<std::string> m_boardPaths;
    int m_lastSelectedIndex = -1;

    ui::Text m_leaderboardTitle;
    std::vector<ui::Text> m_leaderboardScores;
    float m_leaderboardWidth = 0.f;

    void SetupMenuOptions();
    void UpdateLeaderboard();
    void PositionUIElements();

  public:
    BoardSelectionMenuLayer();
    ~BoardSelectionMenuLayer() = default;

    void OnUpdate(float ts) override;
    void OnRender() override;
};