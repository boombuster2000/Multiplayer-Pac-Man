#pragma once

#include "BaseMenuLayer/BaseMenuLayer.h"

class MainMenuLayer : public BaseMenuLayer
{
  private:
    virtual void SetupMenuOptions() override;

  public:
    MainMenuLayer();
    virtual ~MainMenuLayer() = default;
};