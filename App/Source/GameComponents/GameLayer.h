#pragma once
#include "Core/Layer.h"
#include "Core/TexturesManager.h"
#include "Game.h"


class GameLayer : public Core::Layer
{

private:
    Game m_game;

    
public:

    GameLayer();
    virtual ~GameLayer() = default;

    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;

};

