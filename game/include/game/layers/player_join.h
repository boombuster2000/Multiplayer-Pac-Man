#pragma once
#include "engine/core/layer.h"
#include "engine/ui/enums.h"
#include "engine/ui/menu.h"
#include "game/components/board.h"
#include "game/components/pacman.h"
#include "game/components/player_input.h"
#include "game/components/profile.h"
#include <memory>
#include <vector>

class PlayerJoinLayer : public engine::Layer
{

  private:
    void AddJoiningPlayer(PlayerInput* controls);
    void RebuildPlayerMenus();
    static int GetScreenDivisions(const int playerCount);

  private:
    enum class ReadyState
    {
        READY,
        SELECTING_PROFILE
    };

    struct JoiningPlayer
    {
        std::shared_ptr<Profile> profile = nullptr;
        PlayerInput* inputControls = nullptr;
        Pacman pacman;
        ReadyState state = ReadyState::SELECTING_PROFILE;
        ui::Menu profileSelectionMenu;

        explicit JoiningPlayer(PlayerInput* controls, Vector2Ex<float> pacmanSpeed) :
            inputControls(controls),
            pacman({0, 0}, Vector2Ex<float>(50, 50), pacmanSpeed.x), // position needs to be set later
            profileSelectionMenu({0, 0}, ui::AnchorPoint::MIDDLE, ui::Alignment::CENTER, true, 40.f)
        {
        }
    };

  private:
    Board m_board;
    std::vector<JoiningPlayer> m_joiningPlayers{};
    bool m_isFirstPlayerJoined = false;

  public:
    explicit PlayerJoinLayer(const Board& board);

    void OnUpdate(float ts) final;

    void OnRender() final;
};