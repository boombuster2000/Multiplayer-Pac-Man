#pragma once

#include "Core/Layer.h"
#include "UIComponents/Menu.h"

class GameOptionsLayer : public Core::Layer
{
private:
    UIComponents::Menu m_menu;

public:
    GameOptionsLayer();
    virtual ~GameOptionsLayer();

    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;
};