#pragma once

#include "base_menu.h"

class GameOptionsMenuLayer : public BaseMenuLayer
{
  private:
    void SetupMenuOptions() final;

  public:
    GameOptionsMenuLayer();

    void OnUpdate(float ts) final;
    void OnRender() final;
};