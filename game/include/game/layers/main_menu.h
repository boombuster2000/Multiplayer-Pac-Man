#pragma once

#include "base_menu.h"
#include "game/components/player.h"

class MainMenuLayer : public BaseMenuLayer
{
  private:
    virtual void SetupMenuOptions() override;

  public:
    MainMenuLayer();
    virtual ~MainMenuLayer() = default;
};