#pragma once

#include "Core/Layer.h"
#include "UIComponents/Menu.h"

class MainMenuLayer : public Core::Layer
{

  private:
    UIComponents::Menu m_menu;
    bool m_joystickAxisUsed = false;

  public:
    MainMenuLayer();
    virtual ~MainMenuLayer();

    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;
};