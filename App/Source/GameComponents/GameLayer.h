#pragma once
#include "Core/Layer.h"
#include "Core/TexturesManager.h"
#include "UIComponents/Enums.h"
#include "Board.h"
#include "PacMan.h"

class GameLayer : public Core::Layer
{

private:
    Board m_board;
    PacMan m_pacman;
    UIComponents::Direction m_queuedDirection;

private:
    Vector2Ex<float> GetNextValidPacmanPosition(const UIComponents::Direction direction) const;

public:
    GameLayer();
    virtual ~GameLayer() = default;

    void HandleKeyPresses();
    void HandleCollisions();

    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;
};
