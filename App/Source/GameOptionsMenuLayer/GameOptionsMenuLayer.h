#pragma once

#include "BaseMenuLayer/BaseMenuLayer.h"

class GameOptionsMenuLayer : public BaseMenuLayer
{
  private:
    virtual void SetupMenuOptions() override;

  public:
    GameOptionsMenuLayer();
    virtual ~GameOptionsMenuLayer() = default;

    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;
};