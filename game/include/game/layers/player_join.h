#include "engine/core/layer.h"
#include "game/components/client.h"
#include "game/components/pacman.h"
#include "game/components/player_input.h"
#include "game/components/profile.h"
#include <memory>
#include <string_view>
#include <vector>

class PlayerJoinLayer : public engine::Layer
{

  private:
    void AddJoiningPlayer(PlayerInput* controls);
    int GetScreenDivisions(const int playerCount) const;

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
        Pacman pacman = Pacman({0, 0}, Vector2Ex<float>(50, 50), 400); // position needs to be set later
        ReadyState state = ReadyState::SELECTING_PROFILE;
    };

  private:
    std::string_view m_boardPath;
    std::vector<JoiningPlayer> m_joiningPlayers{};
    bool m_isFirstPlayerJoined = false;

  public:
    explicit PlayerJoinLayer(std::string_view boardFilePath);

    void OnUpdate(float ts) final;

    void OnRender() final;
};