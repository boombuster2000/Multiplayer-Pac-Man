#pragma once
#include "Core/Layer.h"
#include "Core/TexturesManager.h"
#include "Board.h"
#include "PacMan.h"


class GameLayer : public Core::Layer
{

private:
    Board m_board;
    PacMan m_pacman;

    
public:

    GameLayer();
    virtual ~GameLayer() = default;

    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;

};

