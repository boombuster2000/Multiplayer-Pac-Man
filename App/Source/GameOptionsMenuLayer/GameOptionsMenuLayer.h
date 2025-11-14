#pragma once

#include "Core/Layer.h"
#include "UIComponents/Menu.h"

class GameOptionsMenuLayer : public Core::Layer
{
  private:
    UIComponents::Menu m_menu;

  public:
    GameOptionsMenuLayer();
    virtual ~GameOptionsMenuLayer();

    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;
};