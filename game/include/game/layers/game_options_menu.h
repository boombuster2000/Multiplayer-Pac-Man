#pragma once

#include "base_menu.h"

class GameOptionsMenuLayer : public BaseMenuLayer
{
  private:
    void SetupMenuOptions() override;

  public:
    GameOptionsMenuLayer();

    void OnUpdate(float ts) override;
    void OnRender() override;
};