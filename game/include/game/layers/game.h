#pragma once
#include "engine/core/layer.h"
#include "engine/core/textures_manager.h"
#include "engine/core/vector2ex.h"
#include "engine/ui/enums.h"
#include "game/components/board.h"
#include "game/components/pacman.h"
#include "game/components/player.h"

class GameLayer : public engine::Layer
{

  private:
    Board m_board;
    Player m_player;
    ui::Direction m_queuedDirection;

    float m_timePassedSinceLastSave = 0.0f;

  private:
    bool IsPacmanTouchingPellet(const Vector2Ex<float>& pacmanDimensions, const Vector2Ex<float>& pacmanPosition) const;
    void CollectPelletAtPosition(const Vector2Ex<float>& position);
    bool CanMoveInDirection(const Vector2Ex<float>& position, const ui::Direction& direction) const;
    bool TryApplyQueuedDirection(Vector2Ex<float>& currentPosition, ui::Direction& currentDirection);
    void RenderScores();

  public:
    GameLayer();
    explicit GameLayer(std::string_view boardPath);
    ~GameLayer() final;

    void HandleKeyPresses();
    void HandleCollisions(const float& deltaTime);
    void UpdateHighscores();

    void OnUpdate(float ts) final;
    void OnRender() final;
};
