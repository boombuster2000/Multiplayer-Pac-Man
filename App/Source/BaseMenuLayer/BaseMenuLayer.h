#pragma once

#include "Core/Layer.h"
#include "UIComponents/Menu.h"

class BaseMenuLayer : public Core::Layer
{
protected:
    UIComponents::Menu m_menu;
    bool m_joystickAxisUsed = false;

    virtual void SetupMenuOptions() = 0;

public:
    BaseMenuLayer(const UIComponents::Alignment alignment, const bool isVisible, const float spacing);
    virtual ~BaseMenuLayer() = default;

    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;
};
