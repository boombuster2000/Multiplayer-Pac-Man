#pragma once

#include "Core/Layer.h"

class OverlayLayer : public Core::Layer
{
public:
    OverlayLayer();
    virtual void OnRender() override;
};