#pragma once

#include "engine/core/layer.h"
#include "engine/ui/menu.h"

class BaseMenuLayer : public engine::Layer
{
  protected:
    ui::Menu m_menu;
    bool m_joystickAxisUsed = false;

    virtual void SetupMenuOptions() = 0;

  public:
    BaseMenuLayer(const ui::Alignment alignment, const bool IsVisible, const float spacing);

    void OnUpdate(float ts) override;
    void OnRender() override;
};
