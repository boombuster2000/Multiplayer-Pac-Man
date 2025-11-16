#pragma once
#include "Core/Layer.h"
#include "Core/TexturesManager.h"
#include "GameComponents/Board.h"
#include "GameComponents/PacMan.h"
#include "UIComponents/Enums.h"

class GameLayer : public Core::Layer
{

  private:
    Board m_board;
    PacMan m_pacman;
    UIComponents::Direction m_queuedDirection;

  private:
    bool CanMoveInDirection(const Vector2Ex<float> &position, const UIComponents::Direction &direction) const;
    bool IsPacmanTouchingPellet(const Vector2Ex<float> &pacmanDimensions, const Vector2Ex<float> &pacmanPosition) const;
    void CollectPelletAtPosition(const Vector2Ex<float> &position);
    bool TryApplyQueuedDirection(Vector2Ex<float> &currentPosition, UIComponents::Direction &currentDirection);

  public:
    GameLayer();
    GameLayer(const std::string &boardPath);
    virtual ~GameLayer() = default;

    void HandleKeyPresses();
    void HandleCollisions(const float &deltaTime);

    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;
};
