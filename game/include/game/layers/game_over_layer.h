#pragma once

#include "engine/core/layer.h"
#include "engine/ui/menu.h"
#include "engine/ui/text.h"
#include "game/components/client.h"
#include <vector>

class GameOverLayer : public engine::Layer
{
  private:
    struct PlayerScoreUI
    {
        ui::Text username;
        ui::Text score;
        float columnWidth;
    };

    ui::Menu m_menu;
    std::vector<PlayerScoreUI> m_playerScoreUIs;
    ui::Text m_title;

    void SetupUI(const std::vector<Client>& clients);

  public:
    explicit GameOverLayer(const std::vector<Client>& clients);

    void OnUpdate(float ts) override;
    void OnRender() override;
};
