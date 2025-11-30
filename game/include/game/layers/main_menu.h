#pragma once

#include "base_menu.h"
#include "game/components/player.h"

class MainMenuLayer : public BaseMenuLayer
{
  private:
    void SetupMenuOptions() final;

  public:
    MainMenuLayer();
};