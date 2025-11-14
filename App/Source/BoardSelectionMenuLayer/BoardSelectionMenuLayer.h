#pragma once

#include "Core/Layer.h"
#include "UIComponents/Menu.h"

class BoardSelectionMenuLayer : public Core::Layer
{
  private:
    UIComponents::Menu m_menu;
    bool m_joystickAxisUsed = false;

  public:
    BoardSelectionMenuLayer();
    virtual ~BoardSelectionMenuLayer();

    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;
};