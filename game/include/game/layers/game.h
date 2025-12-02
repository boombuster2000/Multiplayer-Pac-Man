#pragma once
#include "engine/core/layer.h"
#include "engine/core/textures_manager.h"
#include "engine/core/vector2ex.h"
#include "engine/ui/enums.h"
#include "game/components/board.h"
#include "game/components/entity.h"
#include "game/components/ghost_blinky.h"
#include "game/components/pacman.h"
#include "game/components/player.h"

class GameLayer : public engine::Layer
{

  private:
    Board m_board;
    Player m_player;
    ui::Direction m_queuedDirection;
    Blinky m_blinky;

    float m_timePassedSinceLastSave = 0.0f;

  private:
    bool IsPacmanTouchingPellet(const Vector2Ex<float>& pacmanDimensions, const Vector2Ex<float>& pacmanPosition) const;
    void CollectPelletAtPosition(const Vector2Ex<float>& position);
    bool CanMoveInDirection(Entity* entity, const Vector2Ex<float>& position, const ui::Direction& direction) const;
    bool TryApplyQueuedDirection(Entity* entity, Vector2Ex<float>& currentPosition, ui::Direction& currentDirection);
    void RenderScores() const;
    void RenderNodes() const;

  public:
    GameLayer();
    explicit GameLayer(std::string_view boardPath);
    ~GameLayer() final;

    void HandleKeyPresses();
    void HandleCollisions(Entity* entity, const float& deltaTime, const bool collectPellets);
    void UpdateHighscores();

    void OnUpdate(float ts) final;
    void OnRender() final;
};
