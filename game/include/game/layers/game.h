#pragma once
#include "engine/core/layer.h"
#include "engine/core/textures_manager.h"
#include "engine/core/vector2ex.h"
#include "engine/ui/enums.h"
#include "game/components/board.h"
#include "game/components/client.h"
#include "game/components/entity.h"
#include "game/components/ghost_blinky.h"
#include "game/components/pacman.h"
#include "game/components/player_input.h"

class GameLayer : public engine::Layer
{
  private:
    Board m_board;

    std::vector<Client> m_clients;
    ui::Direction m_queuedDirection;
    Blinky m_blinky;

    float m_timePassedSinceLastSave = 0.0f;

  private:
    bool IsPacmanTouchingPellet(const Pellet& pellet,
                                const Vector2Ex<float>& pacmanDimensions,
                                const Vector2Ex<float>& pacmanPosition) const;

    bool TryCollectPellet(Player& player,
                          const Vector2Ex<float>& pacmanPosition,
                          const Vector2Ex<float>& pacmanDimensions,
                          Pellet& pellet);

    bool TryCollectPellet(Player& player,
                          const Vector2Ex<float>& pacmanPosition,
                          const Vector2Ex<float>& pacmanDimensions,
                          const Vector2Ex<float>& tilePosition);

    void ProcessPelletCollection(Client& client, const Vector2Ex<float> posBefore, const Vector2Ex<float> posAfter);

    bool CanMoveInDirection(Entity* entity, const Vector2Ex<float>& position, const ui::Direction& direction) const;
    bool TryApplyQueuedDirection(Entity* entity, Vector2Ex<float>& currentPosition, ui::Direction& currentDirection);
    void RenderScores() const;
    void RenderNodes() const;

    void SetPacmansSpawnPositions();

    Pacman& GetClosestPacmanWithNodes(const Vector2Ex<float>& referencePoint) const;

    Blinky ConstructBlinky() const;

  public:
    explicit GameLayer(const std::vector<Client>& players);
    GameLayer(const std::vector<Client>& players, std::string_view boardPath);
    ~GameLayer() final;

    void HandleKeyPresses();
    void ProcessMovementSteps(Entity* entity, const float& deltaTime);
    void UpdateHighscores();

    void OnUpdate(float ts) final;
    void OnRender() final;
};
