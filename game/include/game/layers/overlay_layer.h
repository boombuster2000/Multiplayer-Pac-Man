#pragma once

#include "engine/core/layer.h"

class OverlayLayer : public engine::Layer
{
  public:
    OverlayLayer();
    virtual void OnRender() override;
};