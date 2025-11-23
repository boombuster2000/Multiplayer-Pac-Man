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

  private:
    bool IsPacmanTouchingPellet(const Vector2Ex<float>& pacmanDimensions, const Vector2Ex<float>& pacmanPosition) const;
    void CollectPelletAtPosition(const Vector2Ex<float>& position);
    bool CanMoveInDirection(const Vector2Ex<float>& position, const ui::Direction& direction) const;
    bool TryApplyQueuedDirection(Vector2Ex<float>& currentPosition, ui::Direction& currentDirection);
    void RenderScores();

  public:
    GameLayer();
    GameLayer(const std::string& boardPath);
    virtual ~GameLayer() = default;

    void HandleKeyPresses();
    void HandleCollisions(const float& deltaTime);
    void UpdateHighscores();

    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;
};
